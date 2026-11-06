# IRIS CUP 2026 — Divisi Programming

Workspace ROS2 untuk robot mobil otonom berbasis computer vision.

Sesuai regulasi, **kamera adalah satu-satunya sensor yang boleh mendeteksi
lintasan**. Sensor garis seperti IR, TCRT5000, atau QTR array dilarang.
Encoder dan IMU boleh dipakai, asal bukan untuk mendeteksi lintasan.

Lintasan berupa **garis putus-putus kuning** di tengah jalan, dengan dua
garis putih solid sebagai tepi. Satu jalan, tanpa obstacle, 6 checkpoint.
Steering memakai Ackermann dengan satu servo di roda depan.

---

## Struktur workspace

```
iris_fp2/                   <- root workspace, colcon build dari sini
├── build/  install/  log/  <- hasil build, jangan diedit
└── src/
    ├── iris_msgs/          Kontrak pesan antar node
    │   └── msg/            LaneState, MotorCommand
    ├── vision/             Kamera masuk, angka keluar
    │   └── src/vision_node.cpp
    ├── master/             Keputusan gerak
    │   └── src/master_node.cpp
    ├── communication/      Jembatan UDP ke ESP32  (belum diisi)
    └── dashboard/          Basestation monitoring (belum diisi)
```

Empat aturan pembagian tanggung jawab:

1. `vision` tidak boleh tahu apa pun soal servo atau UDP.
2. `master` tidak boleh membuka socket atau memanggil OpenCV.
3. `communication` tidak boleh punya logika kendali, dia cuma transport.
4. Semua angka yang mungkin berubah jadi parameter, bukan ditulis di kode.

---

## Kontrak topic

| Topic | Tipe | Dari | Ke | Status |
|---|---|---|---|---|
| `/lane/state` | `iris_msgs/LaneState` | vision | master | jalan |
| `/robot/cmd` | `iris_msgs/MotorCommand` | master | communication | jalan |
| `/robot/telemetry` | `iris_msgs/Telemetry` | communication | master | belum |

### LaneState.msg

```
std_msgs/Header header
bool found           # apakah lintasan terdeteksi
float32 error        # -1.0 (kiri) .. +1.0 (kanan), 0.0 = tengah
float32 heading      # sudut lintasan, derajat
int32 n_blobs        # jumlah segmen garis putus-putus
```

### MotorCommand.msg

```
std_msgs/Header header
float32 servo        # sudut kemudi derajat, negatif = kiri
int32 speed          # 0-255, diteruskan ESP32 ke STM32
bool enable          # false = motor berhenti
```

---

## Pipeline vision

```
frame kamera
  → warpPerspective (BEV)
  → cvtColor BGR→HSV
  → inRange (isolasi kuning)
  → morphologyEx OPEN lalu CLOSE
  → findContours + filter min_area
  → centroid tiap segmen, diurut dari paling dekat robot
  → error (segmen terdekat) + heading (fit garis semua segmen)
```

**Kenapa BEV.** Karena perspektif, segmen yang jauh terlihat lebih kecil
dan lebih dekat ke tengah daripada posisi aslinya, sehingga `error` jadi
bias terhadap jarak. Setelah di-warp jadi tampak-atas, jarak di gambar
sebanding dengan jarak nyata di lantai, jadi `error` dan `heading` menjadi
besaran geometris yang benar. Bonusnya semua segmen jadi seukuran,
sehingga filter `min_area` lebih bisa diandalkan.

**Kenapa HSV, bukan threshold grayscale.** Garis putus-putus berwarna
kuning, tapi tepi jalan berwarna putih — dan keduanya sama-sama terang.
Threshold berdasarkan kecerahan akan menangkap ketiganya, lalu centroid
terdekat bisa menempel di tepi jalan dan robot membelok ke arah yang
salah. HSV memisahkan keduanya lewat saturasi: putih punya S mendekati
nol, jadi dengan batas bawah S = 120 dia langsung tersingkir.

**Kenapa error dinormalisasi** ke −1.0…+1.0 dan bukan dipakai mentah
dalam piksel: supaya tuning PID tidak berubah saat resolusi kamera atau
`bev_width` diganti.

**Kenapa heading ditahan saat segmen kurang dari dua.** Garis putus-putus
berarti kadang hanya satu segmen yang terlihat, dan satu titik tidak bisa
membentuk garis. Menjatuhkan `heading` ke 0.0 di saat itu berarti
mengatakan "lintasan lurus", padahal yang benar adalah "tidak tahu".
Nilai terakhir dipertahankan supaya tidak ada lonjakan mendadak.

---

## Setup

```bash
sudo apt install ros-humble-cv-bridge python3-opencv \
                 python3-colcon-common-extensions libgl1 libglib2.0-0
```

Tambahkan ke `~/.bashrc` supaya tidak mengetik `source` terus:

```bash
source /opt/ros/humble/setup.bash
source ~/Desktop/iris_fp2/install/setup.bash
```

## Build

**Selalu dari root workspace**, bukan dari home:

```bash
cd ~/Desktop/iris_fp2
colcon build --packages-select vision master
source install/setup.bash
```

Kalau dijalankan dari home, colcon ikut membaca workspace lain dan error
`Duplicate package names not supported`.

## Menjalankan

Terminal 1:
```bash
ros2 run vision vision_node --ros-args -p source:=/path/video.mp4
```

Terminal 2:
```bash
ros2 run master master_node
```

Terminal 3:
```bash
ros2 topic echo /robot/cmd
```

Tiga jendela akan muncul: `vision` (frame asli + trapesium kalibrasi),
`bev` (tampak-atas + overlay deteksi), `mask` (hasil threshold).

---

## Parameter hasil tuning

Diukur dari rekaman `vidio_jalurIrisCup.mp4` (848×478), 3 November 2026.
**Wajib dikalibrasi ulang** saat memakai Logitech C270 di arena.

### vision_node

| Parameter | Nilai | Keterangan |
|---|---|---|
| `source` | `"0"` | index webcam, path video, atau URL stream |
| `hsv_lower` | `[20, 120, 80]` | batas bawah kuning; S=120 membuang garis putih |
| `hsv_upper` | `[35, 255, 255]` | batas atas kuning |
| `bev_top_y` | `0.45` | batas atas trapesium, fraksi tinggi frame |
| `bev_bottom_y` | `1.0` | batas bawah trapesium |
| `bev_top_width` | `0.5` | lebar trapesium di atas |
| `bev_bottom_width` | `1.4` | lebar di bawah; >1 berarti keluar frame, normal |
| `bev_width` | `400` | lebar citra BEV, piksel |
| `bev_height` | `600` | tinggi citra BEV, piksel |
| `min_area` | `60.0` | buang contour lebih kecil dari ini |

### master_node

| Parameter | Nilai | Keterangan |
|---|---|---|
| `kp` | `25.0` | penguatan proporsional |
| `max_angle` | `30.0` | batas mekanis servo, derajat |
| `cruise_speed` | `150` | 0–255 |

Semua bisa diubah saat node berjalan, efeknya langsung terlihat:

```bash
ros2 param set /vision_node bev_top_y 0.42
ros2 param set /master_node kp 20.0
```

### Cara kalibrasi BEV

Atur empat angka trapesium sampai **dua garis putih tepi jalan tampak
sejajar vertikal** di jendela `bev`. Kalau masih menguncup ke atas,
perbesar `bev_bottom_width` atau perkecil `bev_top_width`.

Setelah trapesium berubah, `min_area` hampir selalu perlu disesuaikan
ulang karena ukuran blob ikut berubah.

---

## Perintah debug yang sering dipakai

```bash
ros2 topic hz /lane/state          # vision masih hidup?
ros2 topic echo /robot/cmd         # apa yang dikirim master?
ros2 node list                     # ada node nyasar yang belum dimatikan?
rqt_graph                          # lihat sambungan antar node
```

---

## Progress

- [x] `iris_msgs` dengan `LaneState.msg`
- [x] `vision_node` terbit ke `/lane/state`
- [x] `vision_node` membuka webcam, file video, dan URL
- [x] Bird's Eye View dengan trapesium yang bisa di-tuning live
- [x] Threshold HSV, memisahkan kuning dari garis tepi putih
- [x] Deteksi blob dan perhitungan `error`
- [x] Perhitungan `heading` dari fit garis, dengan penahanan nilai terakhir
- [x] `MotorCommand.msg`
- [x] `master_node` dengan kendali proporsional
- [ ] `Telemetry.msg`
- [ ] `communication` — jembatan UDP ke ESP32
- [ ] `dashboard` — package `ament_python`
- [ ] State machine di master: idle / running / coasting / lost / stopped
- [ ] Suku D pada kendali, dan anti-windup kalau suku I dipakai
- [ ] `params.yaml` dan launch file, supaya tidak `ros2 param set` manual
- [ ] Kalibrasi ulang seluruh parameter dengan Logitech C270
- [ ] Tuning PID di arena (butuh robot fisik)
- [ ] Logika recovery dari checkpoint terdekat

---

## Yang perlu dikonfirmasi

- Aplikasi streaming HP yang akan dipakai panitia, untuk tahu format URL
  (`http://ip:port/video` atau `rtsp://`)
- Format paket UDP, sepakati dengan divisi Electrical
- Apakah benar tidak ada obstacle di arena — jobdesk Mechanical menyebut
  "menghindari rintangan", bagian arena bilang tidak ada
- Tanda `error` dan `heading`: pastikan positif berarti lintasan di kanan,
  sebelum disambungkan ke servo. Tanda terbalik bikin robot membelok
  menjauhi lintasan

---

## Catatan untuk Electrical

Sesuai regulasi, ESP32 dilarang menggerakkan aktuator atau membaca
sensor; dia hanya meneruskan paket ke STM32.

Usulan format paket, 20 Hz dari laptop:

```json
{"seq":1423,"servo":-12.5,"speed":180,"en":1}
```

Balasan dari ESP32, 10 Hz, dengan `seq` yang sama supaya packet loss dan
latency bisa diukur:

```json
{"seq":1423,"rpm":312,"enc":48210,"vbat":3.91}
```

Minta juga **deadman timer**: kalau ESP32 tidak menerima paket selama
500 ms, motor berhenti sendiri. Ini pengaman kalau WiFi arena ngadat di
tengah percobaan.

---

## Strategi lomba

Rumus skor: `Skor = C × [1 + max((300 − T)/300, 0)]`
dengan C = checkpoint yang berhasil ditempuh, T = waktu tempuh dalam detik.

Karena hanya ada 6 checkpoint dan bonus waktu maksimal ×2, **menyelesaikan
semua checkpoint jauh lebih berharga daripada ngebut**. Enam checkpoint
pelan mengalahkan dua checkpoint secepat apa pun. Kejar reliabilitas dulu,
kecepatan belakangan.

Keluar jalur dikurangi 1 poin, tapi boleh mengulang dari checkpoint
terdekat yang pernah dilewati — jadi logika recovery berharga.

---

## Timeline

| Tanggal | Agenda |
|---|---|
| 10 Nov 2026 | Mulai testing robot di arena |
| 14 Nov 2026, 23.59 | Deadline pengumpulan berkas dan robot |
| 15 Nov 2026, 18.00 | MatchDay — presentasi 20 menit, tanya jawab 20 menit, lalu perlombaan |