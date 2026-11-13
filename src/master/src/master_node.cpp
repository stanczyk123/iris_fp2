// Package master: satu-satunya tempat keputusan gerak diambil.
//
// Tidak membuka socket, tidak memanggil OpenCV. Kalau modul ini tetap tipis,
// tuning robot di arena jadi urusan satu file parameter saja.
//
// Langganan:
//   /lane/state        iris_msgs/LaneState
//   /robot/telemetry   iris_msgs/Telemetry   (untuk deteksi link mati)
//   /robot/arm         std_msgs/Bool         (true = aba-aba wasit, false = stop)
// Terbit:
//   /robot/cmd         iris_msgs/MotorCommand
//   /robot/state       std_msgs/String       (untuk dashboard)
//
// DISESUAIKAN DENGAN KALIBRASI ELEKTRIKAL 13 Nov 2026:
//   - Arah: servo + = KANAN, speed + = MAJU (sudah cocok dengan error vision:
//     garis di kanan tengah -> error + -> servo + -> belok kanan).
//   - max_angle 30 = batas servo STM32 (cfg_servo_limit_deg 30).
//   - speed sekarang dalam RPM RODA (udp_bridge speed_is_rpm:=true), PID di STM32
//     menjaga rpm walau baterai turun. Batas aman 500 rpm (duty +/-90 %).
//     Konversi dari nilai lama (duty 0..255, roda 65 mm, maks 524 rpm):
//       cruise 150 -> ~300 rpm (~1,0 m/s), corner 110 -> ~220 rpm, min 90 -> ~180 rpm
//     Kalau udp_bridge dipakai dengan speed_is_rpm:=false, kembalikan ke 150/110/90.
//   - require_telemetry true: ESP32 sudah ada, robot berhenti kalau link mati.

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/string.hpp>

#include "iris_msgs/msg/lane_state.hpp"
#include "iris_msgs/msg/motor_command.hpp"
#include "iris_msgs/msg/telemetry.hpp"

// Garis putus-putus berarti ada jeda WAJAR tanpa garis terlihat. Karena itu
// robot tidak boleh langsung panik saat deteksi hilang satu frame.
enum class State
{
  IDLE,       // menunggu aba-aba wasit
  RUNNING,    // mengikuti lintasan
  COASTING,   // garis hilang sebentar, lanjut perintah terakhir
  LOST,       // garis benar-benar hilang, berhenti
  STOPPED     // dihentikan manual, atau vision/link mati
};

static const char * stateName(State s)
{
  switch (s) {
    case State::IDLE:     return "idle";
    case State::RUNNING:  return "running";
    case State::COASTING: return "coasting";
    case State::LOST:     return "lost";
    default:              return "stopped";
  }
}

class MasterNode : public rclcpp::Node
{
public:
  MasterNode() : Node("master_node")
  {
    kp_ = declare_parameter<double>("kp", 25.0);
    max_angle_ = declare_parameter<double>("max_angle", 30.0);
    deadband_ = declare_parameter<double>("deadband", 0.02);

    // Satuan: rpm roda (lihat catatan di atas). 1 m/s ~ 294 rpm untuk roda 65 mm.
    cruise_ = declare_parameter<int>("cruise_speed", 300);
    corner_ = declare_parameter<int>("corner_speed", 220);
    min_speed_ = declare_parameter<int>("min_speed", 180);

    grace_frames_ = declare_parameter<int>("grace_frames", 15);
    lane_timeout_ms_ = declare_parameter<int>("lane_timeout_ms", 500);
    telem_timeout_ms_ = declare_parameter<int>("telemetry_timeout_ms", 500);
    // ESP32 sudah tersambung (telemetri 20 Hz): robot berhenti kalau WiFi putus.
    // Set false hanya saat tes vision tanpa robot.
    require_telemetry_ = declare_parameter<bool>("require_telemetry", true);

    const int loop_hz = declare_parameter<int>("loop_hz", 20);

    cmd_pub_ = create_publisher<iris_msgs::msg::MotorCommand>("/robot/cmd", 10);
    state_pub_ = create_publisher<std_msgs::msg::String>("/robot/state", 10);

    lane_sub_ = create_subscription<iris_msgs::msg::LaneState>(
      "/lane/state", 10,
      [this](const iris_msgs::msg::LaneState::SharedPtr msg) {
        lane_ = *msg;
        last_lane_ = now();
        have_lane_ = true;
      });

    telem_sub_ = create_subscription<iris_msgs::msg::Telemetry>(
      "/robot/telemetry", 10,
      [this](const iris_msgs::msg::Telemetry::SharedPtr) {
        last_telem_ = now();
        have_telem_ = true;
      });

    arm_sub_ = create_subscription<std_msgs::msg::Bool>(
      "/robot/arm", 10,
      std::bind(&MasterNode::onArm, this, std::placeholders::_1));

    // Timer, bukan dipicu oleh /lane/state. Bedanya penting: kalau vision
    // mati, node ini tetap hidup dan tetap mengirim perintah berhenti.
    // Kalau dipicu callback, dia ikut diam dan robot tidak pernah diberi
    // tahu untuk berhenti.
    timer_ = create_wall_timer(
      std::chrono::milliseconds(1000 / std::max(1, loop_hz)),
      std::bind(&MasterNode::tick, this));

    RCLCPP_INFO(get_logger(),
                "master_node jalan, loop %d Hz. State: IDLE. "
                "Kirim true ke /robot/arm untuk mulai.", loop_hz);
  }

private:
  void onArm(const std_msgs::msg::Bool::SharedPtr msg)
  {
    if (msg->data) {
      state_ = State::RUNNING;
      miss_count_ = 0;
      RCLCPP_INFO(get_logger(), "ARMED");
    } else {
      state_ = State::STOPPED;
      miss_count_ = 0;
      RCLCPP_INFO(get_logger(), "STOPPED");
    }
  }

  bool laneFresh() const
  {
    if (!have_lane_) {
      return false;
    }
    return (now() - last_lane_).seconds() * 1000.0 < lane_timeout_ms_;
  }

  bool linkAlive() const
  {
    if (!require_telemetry_) {
      return true;
    }
    if (!have_telem_) {
      return false;
    }
    return (now() - last_telem_).seconds() * 1000.0 < telem_timeout_ms_;
  }

  void updateState(bool found, bool lane_fresh, bool link_alive)
  {
    // Vision mati atau link mati lebih berbahaya daripada garis hilang:
    // yang pertama berarti robot berjalan buta.
    if ((!lane_fresh || !link_alive) && state_ != State::IDLE) {
      if (state_ != State::STOPPED) {
        RCLCPP_WARN(get_logger(), "%s hilang, robot dihentikan",
                    lane_fresh ? "Link ESP32" : "Vision");
      }
      state_ = State::STOPPED;
      return;
    }

    if (state_ == State::IDLE || state_ == State::STOPPED) {
      return;
    }

    if (found) {
      miss_count_ = 0;
      state_ = State::RUNNING;
    } else {
      ++miss_count_;
      state_ = (miss_count_ <= grace_frames_) ? State::COASTING : State::LOST;
    }
  }

  void tick()
  {
    const bool fresh = laneFresh();
    updateState(fresh && lane_.found, fresh, linkAlive());

    iris_msgs::msg::MotorCommand cmd;
    cmd.header.stamp = now();

    const bool moving = (state_ == State::RUNNING || state_ == State::COASTING);
    if (moving) {
      // Saat COASTING, lane_ masih berisi pembacaan terakhir yang valid.
      // Robot melanjutkan perintah terakhir melewati celah antar garis.
      double error = lane_.error;
      if (std::abs(error) < deadband_) {
        error = 0.0;
      }

      cmd.servo = static_cast<float>(
        std::clamp(kp_ * error, -max_angle_, max_angle_));

      // Melambat sebanding dengan ketajaman situasi. Pakai heading juga,
      // bukan cuma error, supaya robot sudah turun kecepatan SEBELUM masuk
      // tikungan, bukan setelah telanjur melebar.
      const double sharpness = std::min(
        1.0, std::max(std::abs(error), std::abs(lane_.heading) / 45.0));
      const double speed = cruise_ - (cruise_ - corner_) * sharpness;
      cmd.speed = static_cast<int32_t>(std::max<double>(min_speed_, speed));
      cmd.enable = true;
    } else {
      cmd.servo = 0.0f;
      cmd.speed = 0;
      cmd.enable = false;
    }

    cmd_pub_->publish(cmd);

    std_msgs::msg::String s;
    s.data = stateName(state_);
    state_pub_->publish(s);

    if (state_ != last_logged_) {
      RCLCPP_INFO(get_logger(), "state -> %s", stateName(state_));
      last_logged_ = state_;
    }
  }

  double kp_{}, max_angle_{}, deadband_{};
  int cruise_{}, corner_{}, min_speed_{};
  int grace_frames_{}, lane_timeout_ms_{}, telem_timeout_ms_{};
  bool require_telemetry_{false};

  State state_{State::IDLE};
  State last_logged_{State::IDLE};
  int miss_count_{0};

  iris_msgs::msg::LaneState lane_;
  bool have_lane_{false}, have_telem_{false};
  rclcpp::Time last_lane_{0, 0, RCL_ROS_TIME};
  rclcpp::Time last_telem_{0, 0, RCL_ROS_TIME};

  rclcpp::Publisher<iris_msgs::msg::MotorCommand>::SharedPtr cmd_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr state_pub_;
  rclcpp::Subscription<iris_msgs::msg::LaneState>::SharedPtr lane_sub_;
  rclcpp::Subscription<iris_msgs::msg::Telemetry>::SharedPtr telem_sub_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr arm_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MasterNode>());
  rclcpp::shutdown();
  return 0;
}