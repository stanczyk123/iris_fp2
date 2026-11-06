// Package communication: satu-satunya titik sentuh ke ESP32.
//
// Murni transport. Kalau ada baris di sini yang memutuskan robot harus
// belok atau berhenti, baris itu salah tempat; pindahkan ke master_node.
//
// Langganan:
//   /robot/cmd        iris_msgs/MotorCommand  -> dikirim sebagai UDP
// Terbit:
//   /robot/telemetry  iris_msgs/Telemetry     <- diterima dari UDP
//
// Format paket, SEPAKATI DENGAN DIVISI ELECTRICAL:
//   Laptop -> ESP32, 20 Hz:
//     {"seq":1423,"servo":-12.5,"speed":180,"en":1}
//   ESP32 -> Laptop, 10 Hz:
//     {"seq":1423,"rpm":312,"enc":48210,"vbat":3.91}
//
// seq dipantulkan balik ESP32 supaya packet loss dan latency bisa diukur.
// Regulasi: ESP32 dilarang menggerakkan aktuator atau membaca sensor;
// dia hanya meneruskan isi paket ke STM32.

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <thread>

#include <rclcpp/rclcpp.hpp>

#include "iris_msgs/msg/motor_command.hpp"
#include "iris_msgs/msg/telemetry.hpp"

// Parser JSON seadanya: ambil satu angka setelah "key":
// Cukup untuk paket yang bentuknya kita tentukan sendiri, dan menghindari
// menambah dependensi library JSON ke seluruh workspace.
static bool jsonNumber(const std::string & s, const char * key, double & out)
{
  const std::string pat = std::string("\"") + key + "\"";
  const auto k = s.find(pat);
  if (k == std::string::npos) {
    return false;
  }
  const auto colon = s.find(':', k + pat.size());
  if (colon == std::string::npos) {
    return false;
  }
  try {
    out = std::stod(s.substr(colon + 1));
  } catch (const std::exception &) {
    return false;
  }
  return true;
}

class UdpBridgeNode : public rclcpp::Node
{
public:
  UdpBridgeNode() : Node("udp_bridge_node")
  {
    esp_ip_ = declare_parameter<std::string>("esp32_ip", "127.0.0.1");
    esp_port_ = declare_parameter<int>("esp32_port", 4210);
    listen_port_ = declare_parameter<int>("listen_port", 4211);

    if (!openSocket()) {
      throw std::runtime_error("socket UDP gagal disiapkan");
    }

    telem_pub_ =
      create_publisher<iris_msgs::msg::Telemetry>("/robot/telemetry", 10);

    sub_ = create_subscription<iris_msgs::msg::MotorCommand>(
      "/robot/cmd", 10,
      std::bind(&UdpBridgeNode::onCmd, this, std::placeholders::_1));

    running_ = true;
    rx_thread_ = std::thread(&UdpBridgeNode::rxLoop, this);

    RCLCPP_INFO(get_logger(), "udp_bridge_node jalan -> %s:%d, dengar :%d",
                esp_ip_.c_str(), esp_port_, listen_port_);
  }

  ~UdpBridgeNode() override
  {
    // Kirim perintah berhenti sebelum menutup socket, supaya robot tidak
    // melanjutkan perintah terakhir saat program ditutup.
    if (sock_ >= 0) {
      char buf[128];
      const int n = std::snprintf(
        buf, sizeof(buf),
        "{\"seq\":%u,\"servo\":0.00,\"speed\":0,\"en\":0}", seq_ + 1);
      ::sendto(sock_, buf, static_cast<size_t>(n), 0,
               reinterpret_cast<sockaddr *>(&dest_), sizeof(dest_));
    }

    running_ = false;
    if (rx_thread_.joinable()) {
      rx_thread_.join();
    }
    if (sock_ >= 0) {
      ::close(sock_);
    }
  }

private:
  bool openSocket()
  {
    sock_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_ < 0) {
      RCLCPP_FATAL(get_logger(), "socket() gagal: %s", std::strerror(errno));
      return false;
    }

    int reuse = 1;
    ::setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // Timeout supaya rxLoop bisa berhenti saat node ditutup, bukan
    // menggantung selamanya di recvfrom.
    timeval tv{};
    tv.tv_sec = 0;
    tv.tv_usec = 200000;
    ::setsockopt(sock_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    sockaddr_in local{};
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(static_cast<uint16_t>(listen_port_));
    if (::bind(sock_, reinterpret_cast<sockaddr *>(&local),
               sizeof(local)) < 0) {
      RCLCPP_FATAL(get_logger(), "bind(:%d) gagal: %s",
                   listen_port_, std::strerror(errno));
      ::close(sock_);
      sock_ = -1;
      return false;
    }

    dest_.sin_family = AF_INET;
    dest_.sin_port = htons(static_cast<uint16_t>(esp_port_));
    if (::inet_pton(AF_INET, esp_ip_.c_str(), &dest_.sin_addr) != 1) {
      RCLCPP_FATAL(get_logger(), "esp32_ip tidak valid: %s", esp_ip_.c_str());
      ::close(sock_);
      sock_ = -1;
      return false;
    }
    return true;
  }

  void onCmd(const iris_msgs::msg::MotorCommand::SharedPtr cmd)
  {
    char buf[160];
    const int n = std::snprintf(
      buf, sizeof(buf),
      "{\"seq\":%u,\"servo\":%.2f,\"speed\":%d,\"en\":%d}",
      ++seq_, cmd->servo, std::clamp<int>(cmd->speed, 0, 255),
      cmd->enable ? 1 : 0);

    if (::sendto(sock_, buf, static_cast<size_t>(n), 0,
                 reinterpret_cast<sockaddr *>(&dest_), sizeof(dest_)) < 0) {
      RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 2000,
                           "sendto gagal: %s", std::strerror(errno));
    } else {
      ++sent_;
    }
  }

  void rxLoop()
  {
    char buf[512];
    while (running_) {
      const ssize_t n = ::recvfrom(sock_, buf, sizeof(buf) - 1, 0,
                                   nullptr, nullptr);
      if (n <= 0) {
        continue;   // timeout biasa, cek running_ lalu ulangi
      }
      buf[n] = '\0';
      const std::string raw(buf);

      // Paket rusak dibuang diam-diam, bukan bikin node mati di tengah lomba.
      double seq = 0.0, rpm = 0.0, enc = 0.0, vbat = 0.0;
      if (!jsonNumber(raw, "seq", seq)) {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 2000,
                             "Paket telemetri tidak dikenali, dibuang");
        continue;
      }
      jsonNumber(raw, "rpm", rpm);
      jsonNumber(raw, "enc", enc);
      jsonNumber(raw, "vbat", vbat);

      iris_msgs::msg::Telemetry msg;
      msg.header.stamp = now();
      msg.seq = static_cast<uint32_t>(seq);
      msg.rpm = static_cast<float>(rpm);
      msg.enc = static_cast<int32_t>(enc);
      msg.vbat = static_cast<float>(vbat);
      telem_pub_->publish(msg);

      ++received_;

      // Laporkan packet loss sesekali, bukan tiap paket.
      RCLCPP_INFO_THROTTLE(
        get_logger(), *get_clock(), 5000,
        "sent=%u recv=%u loss=%.1f%% vbat=%.2fV",
        sent_, received_,
        sent_ > 0 ? 100.0 * (1.0 - static_cast<double>(received_) / sent_) : 0.0,
        vbat);
    }
  }

  std::string esp_ip_;
  int esp_port_{0};
  int listen_port_{0};
  int sock_{-1};
  sockaddr_in dest_{};

  unsigned seq_{0};
  unsigned sent_{0};
  unsigned received_{0};

  std::atomic<bool> running_{false};
  std::thread rx_thread_;

  rclcpp::Publisher<iris_msgs::msg::Telemetry>::SharedPtr telem_pub_;
  rclcpp::Subscription<iris_msgs::msg::MotorCommand>::SharedPtr sub_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<UdpBridgeNode>());
  rclcpp::shutdown();
  return 0;
}