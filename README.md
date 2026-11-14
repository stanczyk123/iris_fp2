# IRIS CUP 2026 — MAX LAB

Mobil robot pengikut garis otonom. Proyek akhir program magang, divisi Programming.

Kamera di robot mengirim video ke laptop lewat WiFi. Laptop mendeteksi garis,
menghitung sudut setir dan kecepatan, lalu mengirim perintah ke ESP32 lewat UDP.
ESP32 meneruskannya ke STM32 lewat UART, dan STM32 menggerakkan servo dan motor.

```
HP (RTSP) ──WiFi──> laptop ──UDP:4210──> ESP32 ──UART──> STM32 ──> servo + motor
                       ^                    |
                       └────UDP:4211────────┘  telemetri 20 Hz
```

---

## Perangkat

| Bagian | Keterangan |
|---|---|
| Kemudi | Ackermann, dua roda depan membelok, roda belakang menggerakkan |
| Wheelbase | 119 mm |
| Roda | diameter 65 mm, keliling 20,42 cm |
| Servo | SG90, batas aman ±27° |
| Encoder | 422 hitungan per putaran (`cfg_cpr` di `main.c`) |
| Baterai | 3S, 9,0–12,6 V |
| Kamera | HP Android menjalankan aplikasi RTSP, 640x360, 15–30 fps |

Radius belok minimum: `R = L / tan(δ_max)` = 119 mm / tan 27° ≈ 234 mm.

---

## Susunan paket

```
src/
  vision/         vision_node.cpp        BEV, HSV, deteksi garis
  master/         master_node.cpp        kendali P, mesin keadaan
                  config/params.yaml     SEMUA angka tuning ada di sini
                  launch/iris_cup.launch.py
  communication/  udp_bridge_node.cpp    jembatan ROS2 <-> UDP ke ESP32
  dashboard/web/  program.html/js/css    halaman pemantau
  iris_msgs/      LaneState, MotorCommand, Telemetry
firmware/         iris_esp32_*.ino, main.c
```

**`params.yaml` menimpa semua nilai default di kode.** Kalau perilaku robot tidak
cocok dengan yang tertulis di `.cpp`, periksa file ini lebih dulu. Ini sumber
kebingungan paling sering di proyek ini.

---

## Menjalankan

### Prasyarat

1. Robot menyala — baterai terpasang, saklar on, STM32 hidup.
   Kalau STM32 mati, ESP32 tidak punya telemetri untuk dikirim dan terlihat
   seperti tidak tersambung.
2. ESP32 tersambung WiFi yang sama.
3. HP kamera di WiFi yang sama, aplikasi RTSP sudah ditekan **Start**,
   layar jangan terkunci.
4. Laptop di WiFi yang sama.

### Terminal 1 — bersihkan sisa proses

```bash
sudo fuser -k 9090/tcp 8080/tcp 8000/tcp 2>/dev/null
pkill -f master_node; pkill -f udp_bridge_node; pkill -f vision_node
pkill -f rosbridge; pkill -f web_video_server; pkill -f rosapi
ps aux | grep -E "master_node|udp_bridge|vision_node|rosbridge|web_video" | grep -v grep
```

Baris terakhir harus kosong.

### Terminal 1 — pastikan kamera dan ESP32 hidup

```bash
nc -zv <IP_HP> 8554
```

Harus `succeeded`. Perhatikan: `nc` memisahkan host dan port dengan **spasi**,
bukan titik dua.

```bash
python3 - <<'EOF'
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 4211)); s.settimeout(3)
s.sendto(b'{"seq":1,"servo":0.00,"speed":0,"en":0,"mode":1}',
         ('<IP_ESP32>', 4210))
try:
    d, a = s.recvfrom(512); print("BALASAN dari", a, "->", d[:150])
except socket.timeout:
    print("TIDAK ADA BALASAN")
EOF
```

Harus muncul `BALASAN` berisi `vbat`. Jangan pakai `nc` untuk ini — pada UDP,
`nc -zv` sering melaporkan sukses padahal tidak ada yang mendengarkan.

### Terminal 1 — launch, lalu biarkan terbuka

```bash
export ROS_AUTOMATIC_DISCOVERY_RANGE=LOCALHOST
cd ~/Desktop/iris_fp2
source install/setup.bash
ros2 launch master iris_cup.launch.py
```

Empat baris yang harus muncul:

```
Sumber terbuka: rtsp://...:8554/live (640x360) [langsung] via FFMPEG
Rosbridge WebSocket server started on port 9090
udp_bridge_node jalan -> <IP_ESP32>:4210, dengar :4211
sent=... recv=... loss=...% vbat=12.xxV
```

`recv` harus ikut naik mengikuti `sent`. Kalau `recv` diam di 0 dan `loss`
merangkak ke 100%, ESP32 tidak menjawab.

### Terminal 2

```bash
export ROS_AUTOMATIC_DISCOVERY_RANGE=LOCALHOST
cd ~/Desktop/iris_fp2 && source install/setup.bash
ros2 daemon stop
ros2 node list
```

`source` dan `export` harus diulang di **setiap** terminal baru.

### Dashboard

`http://localhost:8000/program.html` → tekan **Connect**

| Yang dicek | Harus |
|---|---|
| Battery | angka, bukan `- V` |
| Tiga panel video | gambar muncul, FPS 15–30 |
| Lane Status | Detected saat kamera melihat garis |
| Robot State | idle |

---

## Jaringan

Tiga alat harus berada di WiFi yang sama: laptop, ESP32, dan HP kamera.

| Nilai | Tempat |
|---|---|
| URL RTSP kamera | `params.yaml` baris `source` |
| IP ESP32 | `params.yaml` baris `esp32_ip` |

**Jangan andalkan `esp32_ip: "auto"` di jaringan kantor.** Penemuan otomatis
memakai beacon UDP broadcast ke `255.255.255.255`, dan router kantor umumnya
membuang paket broadcast. Isi IP-nya manual. Minta elektrikal memasang IP statis
di sketsa ESP32 supaya alamatnya tidak berubah tiap kali router memberi DHCP baru.

Cara cepat memastikan jaringan tidak memblokir alat saling bicara:

```bash
nc -zv <IP_HP> 8554
```

`Connection refused` justru kabar baik — paketmu sampai, cuma belum ada yang
mendengarkan. Yang berbahaya `timed out` atau `No route to host`: itu tanda
client isolation, dan tidak ada yang bisa diperbaiki dari sisi koding.

---

## Parameter

Semua di `src/master/config/params.yaml`.

### vision_node

| Parameter | Arti |
|---|---|
| `source` | URL RTSP, atau `"0"` untuk kamera yang tercolok ke laptop |
| `hsv_lower` / `hsv_upper` | rentang warna garis. Turunkan batas bawah `value` kalau garis di bayangan hilang |
| `bev_top_y` | seberapa jauh ke depan trapesium menjangkau. Makin kecil makin jauh |
| `bev_top_width` | lebar atas trapesium. Di lintasan lurus, garis di BEV harus tegak dan lebarnya tetap |
| `min_area` | luas minimum bercak agar dihitung sebagai garis. Penentu apakah derau ikut terdeteksi |

Semua parameter ini dibaca ulang **tiap frame**, jadi `ros2 param set` berlaku
seketika tanpa launch ulang. Pakai itu untuk mencari angkanya, baru tulis hasil
akhirnya ke `params.yaml`.

### master_node

| Parameter | Arti |
|---|---|
| `kp` | penguatan kendali P: `servo = kp × error`, dibatasi ±`max_angle` |
| `max_angle` | 27°, batas mekanis SG90 |
| `cruise_speed` / `corner_speed` / `min_speed` | rpm roda. Melambat sebanding ketajaman tikungan |
| `grace_frames` | berapa frame robot boleh melanjutkan perintah terakhir saat garis hilang |
| `lane_timeout_ms` / `telemetry_timeout_ms` | ambang berhenti kalau vision atau link mati |

`master_node` membaca parameternya **sekali saat start**. `ros2 param set` tidak
berpengaruh kecuali patch callback parameter sudah dipasang. Tanpa patch itu,
ubah `params.yaml` lalu launch ulang.

Kendalinya **P murni**, bukan PID — tidak ada suku integral maupun turunan.
Kinematika Ackermann tidak dihitung eksplisit; `max_angle` yang menjaga sudut
tetap di dalam batas mekanis.

---

## Pengaman

Tiga lapis, dari paling dalam:

| Lapis | Ambang | Pemicu |
|---|---|---|
| STM32 `failsafe_check()` | 500 ms tanpa perintah | motor berhenti |
| ESP32 `linkWatchdog()` | 1000 ms WiFi putus | kirim `X` ke STM32 |
| `master_node` | `telemetry_timeout_ms` 500 ms | berhenti kirim gerak |

**ESTOP mengunci, `X` tidak.** `ESTOP` menahan robot sampai `GO` dikirim. `X`
sengaja tidak mengunci, karena `linkWatchdog()` mengirim `X` otomatis tiap kali
WiFi tersendat lebih dari satu detik — kalau `X` mengunci, robot akan mati
permanen di tengah lomba.

`stop_reason` 3 (`en=0 dari programmer`) tidak akan pernah muncul: ESP32
menerjemahkan `en=0` menjadi perintah teks `X`, jadi alasannya tercatat sebagai
`perintah X/STOP`.

---

## Masalah yang sering muncul

| Gejala | Sebab | Tindakan |
|---|---|---|
| `ros2 node list` kosong padahal launch jalan | daemon ROS basi | `ros2 daemon stop`, lalu ulangi |
| `Package 'master' not found` | terminal belum di-`source` | `source install/setup.bash` |
| `Address already in use` di 9090/8080/8000 | launch lama belum mati | `sudo fuser -k 9090/tcp 8080/tcp 8000/tcp` |
| `loss` merangkak ke 50% | ada dua publisher ke `/robot/cmd` | `ros2 topic info /robot/cmd`, `Publisher count` harus 1 |
| `recv` diam di 0 | ESP32 atau STM32 mati | tes UDP Python di atas |
| ESP32 tidak ditemukan padahal online | beacon broadcast diblokir router | isi `esp32_ip` manual |
| `Gagal membuka sumber: rtsp://...` + `Unauthorized` | user/password RTSP salah | cek di aplikasi HP, verifikasi dengan `ffprobe` |
| Perubahan `params.yaml` tidak berpengaruh | build bukan `--symlink-install` | `colcon build --symlink-install` |
| Panel video `Waiting for video stream...` | baterai HP habis atau aplikasi berhenti | nyalakan lagi, tidak perlu launch ulang |
| Nilai yang diubah lewat dashboard hilang | parameter dashboard hanya di memori | tulis ke `params.yaml` |

Kotak Kejadian di halaman web ESP32 memberi cap waktu saat **dirender**, bukan
saat kejadiannya. Setelah halaman di-refresh, semua baris lama akan bercap jam
sekarang. Jangan jadikan acuan urutan waktu.

---

## Hasil pengujian

### Tes protokol (`Tes_UDP_dan_Gerak_Programmer_IRIS.docx`)

| Tes | Isi | Hasil |
|---|---|---|
| A | Komunikasi UDP | lulus |
| B | Servo | lulus |
| C | Motor | lulus |
| D | Pengaman | lulus 5/5 setelah perbaikan |
| E | Daya tahan 5 menit | belum lulus |

Tes D menemukan dua bug nyata di firmware:

**Wraparound `uint32_t`.** `failsafe_check()` membandingkan selisih waktu sebagai
bilangan tak bertanda, sehingga pengurangan yang melewati nol menghasilkan angka
raksasa dan fail-safe memicu terus. Diperbaiki dengan membandingkan sebagai
`int32_t`, yang tidak bisa wrap.

**Tombol STOP tidak mengunci.** `X` menghentikan motor, lalu paket 20 Hz
berikutnya menyalakannya lagi 50 ms kemudian. Diperbaiki dengan kunci ESTOP/GO
berlapis tiga: di `handle_line()`, `control_step()`, dan `motor_output()`.

### Hubungan RSSI dan fail-safe

| RSSI | Fail-safe dalam 5 menit |
|---|---|
| −56 dBm | 0 |
| −79 dBm | 28 |

Kekuatan sinyal adalah variabel penentu kelangsungan aliran perintah. Pastikan
antena ESP32 tidak tertutup baterai, logam, atau motor.

### Autonomous (14 November 2026)

Robot berjalan 6,29 m lalu keluar jalur di tikungan. Sebabnya bukan `kp`:
`Segments Detected` hanya 1, artinya kamera cuma melihat satu strip pada satu
waktu. Di antara strip, `found` menjadi false dan robot masuk COASTING,
melanjutkan perintah terakhir lurus selama 0,75 detik — cukup untuk keluar
lintasan di tikungan.

`Lane Heading` terbaca 86,6°, nilai basi: fit garis butuh minimal dua titik, dan
dengan satu blob `heading` memakai nilai terakhir.

Menurunkan `bev_top_y` tidak menolong — ia hanya meregangkan piksel jauh yang
sudah gepeng. Kesimpulannya kamera perlu ditinggikan secara fisik.

---

## Yang belum selesai

1. **Leher kamera baru.** Target: tiga strip terlihat sekaligus di Raw Camera
   Feed. Setelah terpasang, kalibrasi BEV harus diulang.
2. **Tuning `kp`.** Belum bisa dinilai selama `Segments Detected` masih 1.
   Mulai dari `cruise_speed` 90 dan naikkan bertahap setelah bersih.
3. **Tes E diulang** dengan satu publisher dan baterai penuh.
4. **Power bank untuk HP kamera.** Streaming RTSP menguras baterai 20–30% per
   jam. Kalau HP mati di tengah lomba, robot langsung buta.
5. **IP statis ESP32** supaya `params.yaml` tidak perlu diedit tiap pagi.
6. **Latensi kamera.** 15–30 fps lewat WiFi dengan delay beberapa ratus ms.
   Pada 90 rpm (0,31 m/s), delay 300 ms berarti robot maju 9 cm sebelum sempat
   bereaksi. Perlu diangkat ke mentor sebagai risiko.

## Catatan keamanan

URL RTSP beserta user dan password pernah ter-commit ke riwayat GitHub. Risikonya
rendah karena alamatnya IP lokal dan hanya bisa diakses dari dalam jaringan yang
sama, tetapi kalau password yang sama dipakai di tempat lain, ganti di sana.
Mengganti password di aplikasi RTSP lebih murah daripada menulis ulang riwayat git.