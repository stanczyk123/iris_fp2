// IRIS CUP 2026 — vision_node
//
// Perubahan terhadap versi awal:
//   [M1] Blur dipindah ke ruang BGR, sebelum konversi ke HSV.
//   [M2] Sumber yang gagal dibuka tidak lagi membunuh node; dicoba ulang.
//   [M3] Ganti sumber dari dashboard memeriksa hasilnya.
//   [M4] Tanda heading diselaraskan dengan tanda error.
//   [M5] error diambil dari garis hasil fit, bukan dari satu blob terdekat.
//   [M6] Tiap strip kuning diberi kotak hijau dengan titik merah di tengahnya.
//   [M7] Parameter bev_offset_x untuk kamera yang tidak terpasang di tengah.

#include <chrono>
#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
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


// Satu strip kuning yang lolos ambang luas.
struct Strip
{
  cv::Rect kotak;        // [M6] kotak pembatas, digambar hijau
  cv::Point2f pusat;     // [M6] titik tengah, digambar merah
};


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
    declare_parameter<bool>("tampilkan_jendela", false);

    // [M7] Geser trapesium ke kiri atau kanan, dalam pecahan lebar gambar.
    // Positif menggeser ke kanan. Dipakai kalau kamera tidak terpasang tepat
    // di garis tengah robot: tanpa ini, garis selalu terlihat condong ke satu
    // sisi dan robot menariknya terus sepanjang lintasan.
    declare_parameter<double>("bev_offset_x", 0.0);

    // [M2] Dulu di sini ada throw, dan satu kedipan RTSP saat start membunuh
    // seluruh node. Sekarang node tetap hidup dan mencoba lagi di tick().
    if (!bukaSumber()) {
      RCLCPP_WARN(get_logger(),
                  "Sumber belum bisa dibuka, akan dicoba lagi tiap %d ms",
                  JEDA_COBA_MS);
    }

    pub_ = create_publisher<iris_msgs::msg::LaneState>("/lane/state", 10);
    raw_pub_  = create_publisher<sensor_msgs::msg::Image>("/lane/image_raw", 2);
    bev_pub_  = create_publisher<sensor_msgs::msg::Image>("/lane/bev", 2);
    mask_pub_ = create_publisher<sensor_msgs::msg::Image>("/lane/mask", 2);
    timer_ = create_wall_timer(33ms, std::bind(&VisionNode::tick, this));
  }

  ~VisionNode() override { hentikanGrab(); }

private:
  static constexpr int JEDA_COBA_MS = 1000;   // [M2] jeda antar percobaan buka

  // ---------- Pengambilan frame asinkron ----------
  //
  // Tanpa ini, tick() memanggil cap_.read() yang mengambil frame paling
  // DEPAN dari antrian. Karena frame masuk 30 kali per detik sementara
  // pengolahan lebih lambat dari 33 ms, antrian terus menumpuk dan yang
  // diolah selalu gambar basi — latensi tumbuh terus.
  //
  // Dengan thread terpisah, frame lama DITIMPA, bukan diantre.

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
        cap_.set(cv::CAP_PROP_POS_FRAMES, 0);
        if (!cap_.read(f) || f.empty()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          continue;
        }
      }

      // Kunci hanya selama menyalin frame. Kurung kurawal ini penting:
      // tanpa itu mutex masih dipegang saat tidur di bawah, dan tick()
      // ikut terblokir selama 33 ms tiap frame.
      {
        std::lock_guard<std::mutex> kunci(mtx_);
        f.copyTo(frame_baru_);
        ada_frame_ = true;
      }

      // Sumber langsung sudah mengatur lajunya sendiri. File video tidak:
      // cap_.read() mendekode secepat CPU mampu, jadi rekaman habis dalam
      // hitungan detik dan satu inti terpakai penuh.
      if (!live_) {
        const double fps = cap_.get(cv::CAP_PROP_FPS);
        if (fps > 1.0) {
          std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<int>(1000.0 / fps)));
        }
      }
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

    t_coba_ = std::chrono::steady_clock::now();   // [M2]

    const bool index_webcam = (source_.size() == 1 && isdigit(source_[0]));
    const bool rtsp = (source_.rfind("rtsp://", 0) == 0);

    if (index_webcam) {
      cap_.open(std::stoi(source_), cv::CAP_V4L2);
    } else if (rtsp) {
      // Backend TIDAK boleh dipilih otomatis untuk RTSP. Kalau OpenCV
      // memilih GStreamer sendiri, ia merangkai pipeline dengan rtspsrc
      // bawaan yang latency-nya 2000 ms — itu sumber delay dua detik.
      //
      // Opsi di bawah harus terpasang SEBELUM open(), karena FFmpeg
      // membacanya saat membuka stream.
      //   nobuffer, low_delay  : jangan menyangga demi kehalusan
      //   max_delay 0          : jangan menunggu paket yang telat
      //   reorder_queue_size 0 : jangan menyusun ulang urutan paket
      //
      // rtsp_transport sengaja TIDAK dipaksa. Sebagian server RTSP di HP
      // hanya melayani TCP, dan memaksa udp membuat open() gagal total.
      setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS",
             "fflags;nobuffer|flags;low_delay"
             "|max_delay;0|reorder_queue_size;0",
             1);
      cap_.open(source_, cv::CAP_FFMPEG);

      if (!cap_.isOpened()) {
        RCLCPP_WARN(get_logger(),
                    "FFmpeg tidak bisa membuka RTSP, coba GStreamer latency=0");
        const std::string pipa =
            "rtspsrc location=" + source_ +
            " latency=0 drop-on-latency=true protocols=udp"
            " ! rtph264depay ! h264parse ! avdec_h264"
            " ! videoconvert ! video/x-raw,format=BGR"
            " ! appsink drop=true max-buffers=1 sync=false";
        cap_.open(pipa, cv::CAP_GSTREAMER);
      }
    } else {
      cap_.open(source_);
    }

    live_ = index_webcam || rtsp || source_.rfind("http://", 0) == 0;

    if (!cap_.isOpened()) {
      RCLCPP_ERROR(get_logger(), "Gagal membuka sumber: %s", source_.c_str());
      return false;
    }

    cap_.set(cv::CAP_PROP_BUFFERSIZE, 1);

    RCLCPP_INFO(get_logger(), "Sumber terbuka: %s (%dx%d) %s via %s",
                source_.c_str(),
                static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_WIDTH)),
                static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_HEIGHT)),
                live_ ? "[langsung]" : "[file, diputar sesuai FPS]",
                cap_.getBackendName().c_str());

    t_sebelumnya_ = rclcpp::Time(0, 0, RCL_ROS_TIME);
    fps_halus_ = 0.0f;

    mengambil_ = true;
    grab_thread_ = std::thread(&VisionNode::grabLoop, this);
    return true;
  }

  // [M2][M3] Satu tempat untuk semua percobaan buka ulang, dengan jeda
  // supaya log tidak banjir saat kamera memang mati.
  bool pastikanSumberTerbuka()
  {
    if (cap_.isOpened()) {
      return true;
    }
    const auto sekarang = std::chrono::steady_clock::now();
    const auto lewat = std::chrono::duration_cast<std::chrono::milliseconds>(
                         sekarang - t_coba_).count();
    if (lewat < JEDA_COBA_MS) {
      return false;
    }
    return bukaSumber();
  }

  void tick()
  {
    // Sumber bisa diganti dari dashboard saat node sedang jalan.
    const std::string src_baru = get_parameter("source").as_string();
    if (src_baru != source_) {
      RCLCPP_INFO(get_logger(), "Ganti sumber: %s -> %s",
                  source_.c_str(), src_baru.c_str());
      source_ = src_baru;
      // [M3] Dulu hasilnya diabaikan: kalau URL baru salah, kamera lama sudah
      // ditutup dan node diam selamanya tanpa pesan apa pun.
      if (!bukaSumber()) {
        RCLCPP_WARN(get_logger(),
                    "Sumber baru gagal dibuka, mencoba lagi tiap %d ms",
                    JEDA_COBA_MS);
      }
      return;
    }

    if (!pastikanSumberTerbuka()) {
      return;               // [M2] belum ada kamera, jangan olah apa pun
    }

    cv::Mat frame;
    {
      std::lock_guard<std::mutex> kunci(mtx_);
      if (!ada_frame_) {
        return;             // belum ada frame baru sejak tick terakhir
      }
      frame_baru_.copyTo(frame);
      ada_frame_ = false;
    }

    // ---------- Laju pemrosesan ----------
    // Dihitung di sini, bukan di dashboard: angka ini laju frame yang
    // benar-benar diolah, bukan laju gambar yang digambar browser.
    const rclcpp::Time t_sekarang = now();
    if (t_sebelumnya_.nanoseconds() > 0) {
      const double dt = (t_sekarang - t_sebelumnya_).seconds();
      if (dt > 1e-6) {
        const float f = static_cast<float>(1.0 / dt);
        fps_halus_ = (fps_halus_ <= 0.0f) ? f : (0.9f * fps_halus_ + 0.1f * f);
      }
    }
    t_sebelumnya_ = t_sekarang;

    const double min_area = get_parameter("min_area").as_double();

    // ---------- Bird's Eye View ----------
    // Perspektif bikin segmen yang jauh terlihat lebih kecil dan lebih
    // dekat ke tengah daripada posisi aslinya. Setelah di-warp, jarak di
    // gambar sebanding dengan jarak nyata di lantai.
    const double top_y = get_parameter("bev_top_y").as_double();
    const double bot_y = get_parameter("bev_bottom_y").as_double();
    const double top_w = get_parameter("bev_top_width").as_double();
    const double bot_w = get_parameter("bev_bottom_width").as_double();
    const double off_x = get_parameter("bev_offset_x").as_double();   // [M7]
    const int bw = get_parameter("bev_width").as_int();
    const int bh = get_parameter("bev_height").as_int();

    const float W = static_cast<float>(frame.cols);
    const float H = static_cast<float>(frame.rows);

    // Trapesium di gambar kamera: sempit di atas (jauh), lebar di bawah
    // (dekat). Titik bawah boleh keluar frame, itu normal.
    // [M7] off_x menggeser keempat titik bersamaan.
    const double c = 0.5 + off_x;
    const std::vector<cv::Point2f> src_pts = {
      {static_cast<float>((c - top_w / 2) * W), static_cast<float>(top_y * H)},
      {static_cast<float>((c + top_w / 2) * W), static_cast<float>(top_y * H)},
      {static_cast<float>((c + bot_w / 2) * W), static_cast<float>(bot_y * H)},
      {static_cast<float>((c - bot_w / 2) * W), static_cast<float>(bot_y * H)}
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

    // [M1] Blur dilakukan di BGR, SEBELUM konversi ke HSV.
    //
    // Hue itu besaran melingkar: 0 dan 179 bersebelahan. Merata-ratakan H
    // seperti bilangan biasa menghasilkan warna yang tidak ada di gambar
    // asli. Di tepi antara strip kuning dan aspal gelap, blur di ruang HSV
    // melahirkan piksel dengan H dan S di antara keduanya, dan itulah yang
    // bikin pinggiran mask compang-camping.
    cv::Mat bev_halus, hsv, mask;
    cv::GaussianBlur(bev, bev_halus, cv::Size(5, 5), 0);
    cv::cvtColor(bev_halus, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv,
                cv::Scalar(lo[0], lo[1], lo[2]),
                cv::Scalar(hi[0], hi[1], hi[2]), mask);

    // Open membuang bintik noise, close menyambung segmen yang pecah
    cv::Mat k_open  = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::Mat k_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11, 11));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN,  k_open);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, k_close);

    // ---------- Strip: kotak pembatas dan titik tengah ----------
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<Strip> strip;
    for (const auto & k : contours) {
      if (cv::contourArea(k) < min_area) {
        continue;                       // buang bercak noise
      }
      const cv::Moments m = cv::moments(k);
      if (m.m00 == 0.0) {
        continue;
      }
      Strip s;
      s.kotak = cv::boundingRect(k);     // [M6]
      s.pusat = cv::Point2f(static_cast<float>(m.m10 / m.m00),
                            static_cast<float>(m.m01 / m.m00));
      strip.push_back(s);
    }

    // Urut dari paling bawah BEV = paling dekat robot
    std::sort(strip.begin(), strip.end(),
              [](const Strip & a, const Strip & b) {
                return a.pusat.y > b.pusat.y;
              });

    // ---------- Susun pesan ----------
    iris_msgs::msg::LaneState msg;
    msg.header.stamp = now();
    msg.n_blobs = static_cast<int32_t>(strip.size());
    msg.found = !strip.empty();

    const int cx = bw / 2;
    const double y_acuan = static_cast<double>(bh - 1);   // baris terdekat roda

    // Fit garis x = a*y + b. Sengaja y jadi variabel bebas, karena
    // lintasan membentang vertikal di BEV; fit terhadap x akan meledak
    // saat lintasannya lurus.
    bool fit_sah = false;
    double a = 0.0, b = 0.0;
    if (strip.size() >= 2) {
      double sy = 0.0, sx = 0.0, syy = 0.0, sxy = 0.0;
      const double n = static_cast<double>(strip.size());
      for (const auto & s : strip) {
        sy += s.pusat.y;
        sx += s.pusat.x;
        syy += static_cast<double>(s.pusat.y) * s.pusat.y;
        sxy += static_cast<double>(s.pusat.x) * s.pusat.y;
      }
      const double denom = n * syy - sy * sy;
      if (std::abs(denom) > 1e-6) {
        a = (n * sxy - sx * sy) / denom;
        b = (sx - a * sy) / n;
        fit_sah = true;
      }
    }

    if (fit_sah) {
      // [M4] Tanda diselaraskan dengan error: maju berarti y MENGECIL, jadi
      // tikungan ke kanan menghasilkan a negatif. Tanpa tanda minus di sini,
      // heading berlawanan arah dengan error — tidak terasa selama
      // master_node hanya memakai nilai mutlaknya, tapi jadi jebakan begitu
      // heading ikut dipakai untuk menyetir.
      msg.heading = static_cast<float>(-std::atan(a) * 180.0 / CV_PI);
      last_heading_ = msg.heading;
    } else {
      // Kurang dari dua strip berarti "tidak tahu", bukan "lurus".
      // Pertahankan nilai terakhir supaya tidak ada lonjakan mendadak.
      msg.heading = last_heading_;
    }

    // [M5] error diambil dari garis hasil fit, dievaluasi di baris paling
    // bawah BEV — titik yang paling dekat dengan roda depan.
    //
    // Sebelumnya error memakai titik tengah strip terdekat. Pada garis
    // putus-putus nilai itu melompat setiap kali satu strip keluar dari
    // pandangan dan strip berikutnya menggantikannya, dan lompatan itu
    // langsung jadi sentakan setir karena servo = kp * error.
    double x_acuan = static_cast<double>(cx);
    if (msg.found) {
      x_acuan = strip.front().pusat.x;
      if (fit_sah) {
        // Ekstrapolasi bisa menembus tepi gambar kalau semua strip ada di
        // bagian atas BEV. Dikurung supaya error tetap di dalam -1..+1.
        x_acuan = std::clamp(a * y_acuan + b, 0.0, static_cast<double>(bw));
      }
      msg.error = static_cast<float>((x_acuan - cx) / static_cast<double>(cx));
    }

    // ---------- Visualisasi ----------
    // Trapesium digambar di frame asli supaya kalibrasi BEV kelihatan.
    for (int i = 0; i < 4; ++i) {
      cv::line(frame, src_pts[i], src_pts[(i + 1) % 4],
               cv::Scalar(60, 190, 240), 2);
    }

    // Sumbu tengah BEV, acuan nol untuk error.
    cv::line(bev, {cx, 0}, {cx, bh - 1}, cv::Scalar(90, 90, 90), 1);

    // [M6] Tiap strip: kotak HIJAU, titik tengah MERAH.
    for (const auto & s : strip) {
      cv::rectangle(bev, s.kotak, cv::Scalar(0, 255, 0), 2);
      cv::circle(bev, cv::Point(cvRound(s.pusat.x), cvRound(s.pusat.y)), 4,
                 cv::Scalar(0, 0, 255), -1);
    }

    // Garis hasil fit, supaya kalibrasi BEV bisa diperiksa dengan mata:
    // di lintasan lurus garis ini harus tegak dan melewati semua titik merah.
    if (fit_sah) {
      cv::line(bev,
               cv::Point(cvRound(b), 0),
               cv::Point(cvRound(a * y_acuan + b), bh - 1),
               cv::Scalar(255, 200, 60), 1);
    }

    // Seberapa jauh acuan menyimpang dari sumbu tengah, di baris terdekat roda.
    if (msg.found) {
      cv::line(bev, {cx, bh - 1},
               cv::Point(cvRound(x_acuan), bh - 1),
               cv::Scalar(120, 220, 120), 3);
    }

    char info[128];
    std::snprintf(info, sizeof(info), "err %+.3f  head %+.1f  blobs %d  fps %.1f",
                  msg.error, msg.heading, msg.n_blobs, fps_halus_);
    cv::putText(bev, info, {10, 24}, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                cv::Scalar(240, 240, 240), 1, cv::LINE_AA);

    // FPS juga ditempel di frame mentah, supaya terbaca walau hanya
    // jendela "raw" yang dibuka saat mengukur latensi.
    char fps_teks[32];
    std::snprintf(fps_teks, sizeof(fps_teks), "fps %.1f", fps_halus_);
    cv::putText(frame, fps_teks, {10, 28}, cv::FONT_HERSHEY_SIMPLEX, 0.7,
                cv::Scalar(240, 240, 240), 2, cv::LINE_AA);

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

    // Jendela OpenCV hanya untuk debug dan pengukuran latensi.
    // waitKey(1) memakan 5-15 ms tiap frame, jadi matikan saat lomba.
    if (get_parameter("tampilkan_jendela").as_bool()) {
      cv::imshow("raw", frame);
      cv::imshow("bev", bev);
      cv::imshow("mask", mask);
      cv::waitKey(1);
    }

    pub_->publish(msg);
  }

  std::string source_;
  float last_heading_{0.0f};
  rclcpp::Time t_sebelumnya_{0, 0, RCL_ROS_TIME};
  float fps_halus_{0.0f};
  cv::VideoCapture cap_;
  std::chrono::steady_clock::time_point t_coba_{};   // [M2]

  // Slot satu frame yang terus ditimpa thread pengambil.
  std::thread grab_thread_;
  std::mutex mtx_;
  cv::Mat frame_baru_;
  std::atomic<bool> ada_frame_{false};
  std::atomic<bool> mengambil_{false};
  std::atomic<bool> live_{false};      // true = webcam/RTSP, false = file video

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