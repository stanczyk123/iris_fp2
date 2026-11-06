#include <chrono>
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <sensor_msgs/msg/image.hpp>
#include <mutex>
#include <thread>
#include <atomic>

#if __has_include(<cv_bridge/cv_bridge.hpp>)
#include <cv_bridge/cv_bridge.hpp>
#else
#include <cv_bridge/cv_bridge.h>
#endif

#include <opencv2/opencv.hpp>
#include <rclcpp/rclcpp.hpp>

#include "iris_msgs/msg/lane_state.hpp"

using namespace std::chrono_literals;


class VisionNode : public rclcpp::Node
{
public:
  VisionNode() : Node("vision_node")
  {
    source_ = declare_parameter<std::string>("source", "0");
    declare_parameter<std::vector<int64_t>>("hsv_lower",
                                            std::vector<int64_t>{20, 80, 80});
    declare_parameter<std::vector<int64_t>>("hsv_upper",
                                            std::vector<int64_t>{35, 255, 255});
    declare_parameter<double>("bev_top_y", 0.55);
    declare_parameter<double>("bev_bottom_y", 1.0);
    declare_parameter<double>("bev_top_width", 0.5);
    declare_parameter<double>("bev_bottom_width", 1.4);
    declare_parameter<int>("bev_width", 400);
    declare_parameter<int>("bev_height", 600);
    declare_parameter<double>("min_area", 120.0);

    if (!bukaSumber()) {
      throw std::runtime_error("sumber kamera gagal dibuka");
    }

    pub_ = create_publisher<iris_msgs::msg::LaneState>("/lane/state", 10);
    raw_pub_  = create_publisher<sensor_msgs::msg::Image>("/lane/raw", 2);
    bev_pub_  = create_publisher<sensor_msgs::msg::Image>("/lane/bev", 2);
    mask_pub_ = create_publisher<sensor_msgs::msg::Image>("/lane/mask", 2);
    timer_ = create_wall_timer(33ms, std::bind(&VisionNode::tick, this));
  }

  ~VisionNode() override { hentikanGrab(); }

private:
  // ---------- Pengambilan frame asinkron ----------
  //
  // Tanpa ini, tick() memanggil cap_.read() yang mengambil frame paling
  // DEPAN dari antrian. Karena frame masuk 30 kali per detik sementara
  // pengolahan (warp, HSV, kontur) lebih lambat dari 33 ms, antrian terus
  // menumpuk dan yang diolah selalu gambar basi — latensi tumbuh terus.
  //
  // Dengan thread terpisah, frame lama DITIMPA, bukan diantre. Pengolahan
  // selalu mendapat gambar terkini.

  void hentikanGrab()
  {
    mengambil_ = false;
    if (grab_thread_.joinable()) {
      grab_thread_.join();
    }
  }

  void grabLoop()
  {
    cv::Mat f;
    while (mengambil_) {
      if (!cap_.read(f) || f.empty()) {
        // Untuk file video: ulang dari awal. Untuk stream yang putus:
        // set() gagal tanpa efek, lalu kita tunggu sebentar dan coba lagi.
        cap_.set(cv::CAP_PROP_POS_FRAMES, 0);
        if (!cap_.read(f) || f.empty()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          continue;
        }
      }
      std::lock_guard<std::mutex> kunci(mtx_);
      f.copyTo(frame_baru_);
      ada_frame_ = true;
    }
  }

   bool bukaSumber()
  {
    hentikanGrab();
    cap_.release();

    {
      std::lock_guard<std::mutex> kunci(mtx_);
      ada_frame_ = false;
    }

    if (source_.size() == 1 && isdigit(source_[0])) {
      cap_.open(std::stoi(source_));      // index webcam
    } else {
      cap_.open(source_);                 // file, RTSP, atau URL lain
    }

    if (!cap_.isOpened()) {
      RCLCPP_ERROR(get_logger(), "Gagal membuka sumber: %s", source_.c_str());
      return false;
    }

    cap_.set(cv::CAP_PROP_BUFFERSIZE, 1);

    RCLCPP_INFO(get_logger(), "Sumber terbuka: %s (%dx%d)", source_.c_str(),
                static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_WIDTH)),
                static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_HEIGHT)));

    mengambil_ = true;
    grab_thread_ = std::thread(&VisionNode::grabLoop, this);
    return true;
  }
  
  void tick()
  {
    // Sumber bisa diganti dari dashboard saat node sedang jalan.
    const std::string src_baru = get_parameter("source").as_string();
    if (src_baru != source_) {
      RCLCPP_INFO(get_logger(), "Ganti sumber: %s -> %s",
                  source_.c_str(), src_baru.c_str());
      source_ = src_baru;
      bukaSumber();
      return;
    }

    // Ambil frame TERBARU dari slot, bukan yang berikutnya dalam antrian.
    cv::Mat frame;
    {
      std::lock_guard<std::mutex> kunci(mtx_);
      if (!ada_frame_) {
        return;             // belum ada frame baru sejak tick terakhir
      }
      frame_baru_.copyTo(frame);
      ada_frame_ = false;
    }

    const double min_area = get_parameter("min_area").as_double();

    // ---------- Bird's Eye View ----------
    // Perspektif bikin segmen yang jauh terlihat lebih kecil dan lebih
    // dekat ke tengah daripada posisi aslinya. Setelah di-warp, jarak di
    // gambar sebanding dengan jarak nyata di lantai, jadi error dan
    // heading jadi besaran geometris yang benar.
    const double top_y = get_parameter("bev_top_y").as_double();
    const double bot_y = get_parameter("bev_bottom_y").as_double();
    const double top_w = get_parameter("bev_top_width").as_double();
    const double bot_w = get_parameter("bev_bottom_width").as_double();
    const int bw = get_parameter("bev_width").as_int();
    const int bh = get_parameter("bev_height").as_int();

    const float W = static_cast<float>(frame.cols);
    const float H = static_cast<float>(frame.rows);

    // Trapesium di gambar kamera: sempit di atas (jauh), lebar di bawah
    // (dekat). Titik bawah boleh keluar frame, itu normal.
    const std::vector<cv::Point2f> src_pts = {
      {static_cast<float>((0.5 - top_w / 2) * W), static_cast<float>(top_y * H)},
      {static_cast<float>((0.5 + top_w / 2) * W), static_cast<float>(top_y * H)},
      {static_cast<float>((0.5 + bot_w / 2) * W), static_cast<float>(bot_y * H)},
      {static_cast<float>((0.5 - bot_w / 2) * W), static_cast<float>(bot_y * H)}
    };
    const std::vector<cv::Point2f> dst_pts = {
      {0.0f, 0.0f},
      {static_cast<float>(bw), 0.0f},
      {static_cast<float>(bw), static_cast<float>(bh)},
      {0.0f, static_cast<float>(bh)}
    };

    const cv::Mat M = cv::getPerspectiveTransform(src_pts, dst_pts);
    cv::Mat bev;
    cv::warpPerspective(frame, bev, M, cv::Size(bw, bh));

    // ---------- Threshold warna ----------
    // Garis putus-putus KUNING, garis tepi jalan PUTIH. HSV memisahkan
    // keduanya lewat saturasi: putih punya S mendekati nol.
    const auto lo = get_parameter("hsv_lower").as_integer_array();
    const auto hi = get_parameter("hsv_upper").as_integer_array();

    cv::Mat hsv, mask;
    cv::cvtColor(bev, hsv, cv::COLOR_BGR2HSV);
    cv::GaussianBlur(hsv, hsv, cv::Size(5, 5), 0);
    cv::inRange(hsv,
                cv::Scalar(lo[0], lo[1], lo[2]),
                cv::Scalar(hi[0], hi[1], hi[2]), mask);

    // Open membuang bintik noise, close menyambung segmen yang pecah
    cv::Mat k_open  = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::Mat k_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11, 11));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,  k_open);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, k_close);

    // ---------- Centroid tiap segmen ----------
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point2f> pts;
    for (const auto & c : contours) {
      if (cv::contourArea(c) < min_area) {
        continue;                       // buang bercak noise
      }
      const cv::Moments m = cv::moments(c);
      if (m.m00 == 0.0) {
        continue;
      }
      // Koordinat sudah dalam ruang BEV, tidak perlu offset apa pun.
      pts.emplace_back(m.m10 / m.m00, m.m01 / m.m00);
    }

    // Urut dari paling bawah BEV = paling dekat robot
    std::sort(pts.begin(), pts.end(),
              [](const cv::Point2f & a, const cv::Point2f & b) {
                return a.y > b.y;
              });

    // ---------- Susun pesan ----------
    iris_msgs::msg::LaneState msg;
    msg.header.stamp = now();
    msg.n_blobs = static_cast<int32_t>(pts.size());
    msg.found = !pts.empty();

    const int cx = bw / 2;
    if (msg.found) {
      // Dinormalisasi -1.0 .. +1.0 supaya tuning PID tidak berubah saat
      // bev_width diganti.
      msg.error = (pts.front().x - cx) / static_cast<float>(cx);
    }

    if (pts.size() >= 2) {
      // Fit garis x = a*y + b. Sengaja y jadi variabel bebas, karena
      // lintasan membentang vertikal di BEV; fit terhadap x akan meledak
      // saat lintasannya lurus.
      double sy = 0.0, sx = 0.0, syy = 0.0, sxy = 0.0;
      const double n = static_cast<double>(pts.size());
      for (const auto & p : pts) {
        sy += p.y;
        sx += p.x;
        syy += static_cast<double>(p.y) * p.y;
        sxy += static_cast<double>(p.x) * p.y;
      }
      const double denom = n * syy - sy * sy;
      if (std::abs(denom) > 1e-6) {
        const double a = (n * sxy - sx * sy) / denom;
        msg.heading = static_cast<float>(std::atan(a) * 180.0 / CV_PI);
        last_heading_ = msg.heading;
      } else {
        msg.heading = last_heading_;
      }
    } else {
      // Kurang dari dua segmen berarti "tidak tahu", bukan "lurus".
      // Pertahankan nilai terakhir supaya tidak ada lonjakan mendadak.
      msg.heading = last_heading_;
    }

    // ---------- Visualisasi ----------
    // Trapesium digambar di frame asli supaya kalibrasi BEV kelihatan.
    for (int i = 0; i < 4; ++i) {
      cv::line(frame, src_pts[i], src_pts[(i + 1) % 4],
               cv::Scalar(60, 190, 240), 2);
    }

    cv::line(bev, {cx, bh}, {cx, bh - 40}, cv::Scalar(150, 150, 150), 1);
    for (size_t i = 0; i < pts.size(); ++i) {
      cv::circle(bev, cv::Point(cvRound(pts[i].x), cvRound(pts[i].y)), 6,
                 i == 0 ? cv::Scalar(120, 220, 120) : cv::Scalar(60, 190, 240),
                 -1);
    }
    if (msg.found) {
      cv::line(bev, {cx, bh - 1},
               cv::Point(cvRound(pts.front().x), cvRound(pts.front().y)),
               cv::Scalar(120, 220, 120), 2);
    }

    char info[96];
    std::snprintf(info, sizeof(info), "err %+.3f  head %+.1f  blobs %d",
                  msg.error, msg.heading, msg.n_blobs);
    cv::putText(bev, info, {10, 24}, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                cv::Scalar(240, 240, 240), 1, cv::LINE_AA);

    // Encode hanya kalau ada yang menonton, supaya tidak membuang CPU
    // saat dashboard tidak dibuka.
    if (raw_pub_->get_subscription_count() > 0) {
      raw_pub_->publish(*cv_bridge::CvImage(msg.header, "bgr8", frame).toImageMsg());
    }
    if (bev_pub_->get_subscription_count() > 0) {
      bev_pub_->publish(*cv_bridge::CvImage(msg.header, "bgr8", bev).toImageMsg());
    }
    if (mask_pub_->get_subscription_count() > 0) {
      mask_pub_->publish(*cv_bridge::CvImage(msg.header, "mono8", mask).toImageMsg());
    }

    cv::imshow("vision", frame);
    cv::imshow("bev", bev);
    cv::imshow("mask", mask);
    cv::waitKey(1);

    pub_->publish(msg);
  }

  std::string source_;
  float last_heading_{0.0f};
  cv::VideoCapture cap_;

  // Slot satu frame yang terus ditimpa thread pengambil.
  std::thread grab_thread_;
  std::mutex mtx_;
  cv::Mat frame_baru_;
  std::atomic<bool> ada_frame_{false};
  std::atomic<bool> mengambil_{false};

  rclcpp::Publisher<iris_msgs::msg::LaneState>::SharedPtr pub_;
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr raw_pub_, bev_pub_, mask_pub_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VisionNode>());
  rclcpp::shutdown();
  return 0;
}