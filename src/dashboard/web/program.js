// IRIS Base Station — penghubung halaman web ke ROS2.
//
// Empat mode kerja, dipilih lewat Connection Type:
//   robot     ROS2 jalan, sumber kamera sesuai params.yaml
//   rekaman   ROS2 jalan, sumber diganti ke file video
//   webcam    ROS2 jalan, sumber diganti ke webcam laptop
//   simulasi  tanpa ROS2 sama sekali, data dibangkitkan di browser

// ---------- Kalibrasi jarak tempuh ----------
const COUNT_PER_PUTARAN = 1320;   // hitungan encoder per satu putaran roda
const KELILING_RODA_CM = 20.4;    // roda diameter 6.5 cm -> keliling ~20.4 cm

const MAX_SUDUT = 30;             // batas mekanis servo, derajat

// ---------- Pembantu ----------

const el = (id) => document.getElementById(id);

const statusEl = el("status");
const logEl = el("log");

function log(teks, jenis = "info") {
    const waktu = new Date().toLocaleTimeString("id-ID", { hour12: false });
    const baris = document.createElement("div");
    baris.innerHTML =
        `<span class="waktu">[${waktu}]</span><span class="${jenis}">${teks}</span>`;
    logEl.appendChild(baris);
    logEl.scrollTop = logEl.scrollHeight;
    while (logEl.children.length > 200) {
        logEl.removeChild(logEl.firstChild);
    }
}

function setStatus(teks, kelas) {
    statusEl.textContent = teks;
    statusEl.className = "status " + (kelas || "");
}

// ---------- Gauge sudut kemudi ----------

const PANJANG_BUSUR = 251.3;   // keliling setengah lingkaran r=80

function gambarGauge(sudut) {
    const batas = Math.max(-MAX_SUDUT, Math.min(MAX_SUDUT, sudut));
    el("jarum").style.transform = `rotate(${batas / MAX_SUDUT * 90}deg)`;

    const porsi = (batas + MAX_SUDUT) / (MAX_SUDUT * 2);
    el("busur").style.strokeDashoffset = PANJANG_BUSUR * (1 - porsi);
    el("busur").style.stroke =
        Math.abs(batas) > MAX_SUDUT * 0.75 ? "#facc15" : "#4ade80";

    el("sudut").textContent = batas.toFixed(1) + "°";
}

gambarGauge(0);

// ---------- Tampilkan kolom sesuai mode ----------

function perbaruiForm() {
    const mode = el("tipe").value;
    const butuhRos = mode !== "simulasi";

    el("baris_ros").style.display = butuhRos ? "" : "none";
    el("baris_video").style.display = butuhRos ? "" : "none";
    el("baris_rekaman").style.display = mode === "rekaman" ? "" : "none";
}

el("tipe").addEventListener("change", perbaruiForm);
perbaruiForm();

// ---------- Aliran video ----------

function pasangVideo(host) {
    const pasang = (idImg, idTunggu, idRes, topic) => {
        const img = el(idImg);
        img.onload = () => {
            el(idTunggu).style.display = "none";
            img.classList.add("tampil");
            el(idRes).textContent = img.naturalWidth + "x" + img.naturalHeight;
        };
        img.onerror = () => {
            el(idTunggu).textContent = "Stream tidak tersedia";
            img.classList.remove("tampil");
        };
        img.src = `${host}/stream?topic=${topic}`;
    };

    pasang("img_raw", "tunggu_raw", "res_raw", "/lane/image_raw");
    pasang("img_bev", "tunggu_bev", "res_bev", "/lane/bev");
    pasang("img_mask", "tunggu_mask", "res_mask", "/lane/mask");
}

function lepasVideo() {
    ["img_raw", "img_bev", "img_mask"].forEach((id) => {
        const img = el(id);
        img.src = "";
        img.classList.remove("tampil");
    });
    ["tunggu_raw", "tunggu_bev", "tunggu_mask"].forEach((id) => {
        el(id).style.display = "";
        el(id).textContent = "Waiting for video stream...";
    });
}

// ---------- Penghitung frekuensi ----------

function pembuatFps(idTampil) {
    let hitung = 0;
    let mulai = performance.now();
    return () => {
        hitung++;
        const selisih = performance.now() - mulai;
        if (selisih >= 1000) {
            el(idTampil).textContent = (hitung * 1000 / selisih).toFixed(0);
            hitung = 0;
            mulai = performance.now();
        }
    };
}

const fpsRaw = pembuatFps("fps_raw");
const fpsBev = pembuatFps("fps_bev");
const fpsMask = pembuatFps("fps_mask");

function hitungSemuaFps() {
    fpsRaw();
    fpsBev();
    fpsMask();
}

// ---------- Ganti sumber kamera dari dashboard ----------
// Memanggil service parameter milik vision_node. vision_node akan
// membuka ulang kameranya begitu nilai ini berubah.

function gantiSumber(nilai) {
    const svc = new ROSLIB.Service({
        ros: ros,
        name: "/vision_node/set_parameters",
        serviceType: "rcl_interfaces/srv/SetParameters"
    });

    const permintaan = new ROSLIB.ServiceRequest({
        parameters: [{
            name: "source",
            value: { type: 4, string_value: nilai }   // 4 = string
        }]
    });

    svc.callService(permintaan, (hasil) => {
        const ok = hasil.results && hasil.results[0] &&
                   hasil.results[0].successful;
        if (ok) {
            log("Sumber kamera diganti ke: " + nilai, "ok");
        } else {
            log("Gagal mengganti sumber kamera", "bad");
        }
    }, () => {
        log("vision_node tidak menjawab permintaan ganti sumber", "bad");
    });
}

// ---------- Sambungan ROS2 ----------

let ros = null;
let armTopic = null;
let simTimer = null;

function sambung() {
    const mode = el("tipe").value;

    if (mode === "simulasi") {
        putus(true);
        mulaiSimulasi();
        return;
    }

    const url = el("url_ros").value.trim();
    setStatus("Connecting...", "proses");
    log("Menghubungkan ke " + url, "info");

    ros = new ROSLIB.Ros({ url: url });

    ros.on("connection", () => {
        setStatus("Connected", "ok");
        log("Terhubung ke rosbridge", "ok");
        pasangVideo(el("url_video").value.trim());
        berlangganan();

        // Ganti sumber kamera sesuai mode yang dipilih.
        if (mode === "rekaman") {
            const path = el("path_rekaman").value.trim();
            if (path) {
                gantiSumber(path);
            } else {
                log("Path rekaman kosong, memakai nilai params.yaml", "warn");
            }
        } else if (mode === "webcam") {
            gantiSumber("0");
        }
    });

    ros.on("error", () => {
        setStatus("Connection failed", "gagal");
        log("Gagal terhubung. Pastikan rosbridge sudah jalan.", "bad");
    });

    ros.on("close", () => {
        setStatus("Disconnected", "gagal");
        log("Sambungan terputus", "warn");
    });
}

function putus(diam) {
    if (ros) {
        ros.close();
        ros = null;
    }
    armTopic = null;
    clearInterval(simTimer);
    simTimer = null;
    lepasVideo();
    setStatus("Disconnected", "");
    if (!diam) { log("Diputus oleh pengguna", "warn"); }
}

// ---------- Berlangganan topic ----------

function berlangganan() {
    new ROSLIB.Topic({
        ros: ros,
        name: "/lane/state",
        messageType: "iris_msgs/msg/LaneState"
    }).subscribe((m) => {
        hitungSemuaFps();
        el("error").textContent = m.error.toFixed(3);
        el("heading").textContent = m.heading.toFixed(1) + "°";
        el("blobs").textContent = m.n_blobs;

        const pil = el("lane_status");
        pil.textContent = m.found ? "Detected" : "Lost";
        pil.className = "pil " + (m.found ? "hijau" : "merah");
    });

    new ROSLIB.Topic({
        ros: ros,
        name: "/robot/cmd",
        messageType: "iris_msgs/msg/MotorCommand"
    }).subscribe((m) => {
        gambarGauge(m.servo);
        el("speed").textContent = m.enable ? m.speed : 0;
    });

    new ROSLIB.Topic({
        ros: ros,
        name: "/robot/telemetry",
        messageType: "iris_msgs/msg/Telemetry"
    }).subscribe((m) => {
        el("rpm").textContent = m.rpm.toFixed(0);
        el("vbat").textContent = m.vbat.toFixed(2) + " V";

        const jarakM = m.enc / COUNT_PER_PUTARAN * KELILING_RODA_CM / 100;
        el("jarak").textContent = jarakM.toFixed(2) + " m";

        const persen = Math.max(0, Math.min(100, (m.vbat - 9.0) / 3.6 * 100));
        const bar = el("bar_vbat");
        bar.style.width = persen + "%";
        bar.style.background =
            m.vbat < 10.5 ? "#f87171" : (m.vbat < 11.1 ? "#facc15" : "#4ade80");
    });

    let stateLama = "";
    new ROSLIB.Topic({
        ros: ros,
        name: "/robot/state",
        messageType: "std_msgs/msg/String"
    }).subscribe((m) => {
        const pil = el("state");
        pil.textContent = m.data;
        pil.className = "pil " + warnaState(m.data);
        if (m.data !== stateLama) {
            log("State robot: " + m.data.toUpperCase(),
                m.data === "running" ? "ok" : "warn");
            stateLama = m.data;
        }
    });

    armTopic = new ROSLIB.Topic({
        ros: ros,
        name: "/robot/arm",
        messageType: "std_msgs/msg/Bool"
    });

    // /rosout berisi SELURUH pesan log dari semua node ROS2 — isi terminal,
    // persis seperti yang kamu lihat saat menjalankan ros2 launch.
    new ROSLIB.Topic({
        ros: ros,
        name: "/rosout",
        messageType: "rcl_interfaces/msg/Log"
    }).subscribe((m) => {
        // level: 10 debug, 20 info, 30 warn, 40 error, 50 fatal
        if (m.level < 20) { return; }           // debug terlalu berisik
        const jenis = m.level >= 40 ? "bad"
                    : (m.level >= 30 ? "warn" : "info");
        log(`<span class="node">[${m.name}]</span> ${m.msg}`, jenis);
    });

    log("Berlangganan topic dan log sistem", "ok");
}

function warnaState(s) {
    if (s === "running") { return "hijau"; }
    if (s === "coasting") { return "kuning"; }
    if (s === "lost" || s === "stopped") { return "merah"; }
    return "";
}

// ---------- Mode simulasi ----------
// Tanpa ROS2 sama sekali. Berguna saat mengerjakan tata letak halaman,
// atau saat memperagakan dashboard di laptop yang tidak ada ROS2-nya.

function mulaiSimulasi() {
    setStatus("Simulation", "proses");
    log("Mode simulasi — data dibangkitkan di browser", "warn");

    let t = 0;
    let enc = 0;
    el("lane_status").textContent = "Detected";
    el("lane_status").className = "pil hijau";
    el("state").textContent = "running";
    el("state").className = "pil hijau";

    simTimer = setInterval(() => {
        t += 0.05;
        const err = Math.sin(t) * 0.45;
        const head = Math.sin(t * 0.7) * 22;

        hitungSemuaFps();
        el("error").textContent = err.toFixed(3);
        el("heading").textContent = head.toFixed(1) + "°";
        el("blobs").textContent = 2 + (Math.sin(t * 2) > 0.6 ? 1 : 0);

        gambarGauge(err * 25);
        el("speed").textContent = Math.round(150 - Math.abs(err) * 40);

        enc += 24;
        el("rpm").textContent = (360 + Math.sin(t * 3) * 8).toFixed(0);
        el("jarak").textContent =
            (enc / COUNT_PER_PUTARAN * KELILING_RODA_CM / 100).toFixed(2) + " m";

        const vbat = 4.15 - t * 0.002;
        el("vbat").textContent = vbat.toFixed(2) + " V";
        el("bar_vbat").style.width =
            Math.max(0, Math.min(100, (vbat - 3.2) * 100)) + "%";
    }, 50);
}

// ---------- Tombol ----------

el("btn_connect").addEventListener("click", sambung);
el("btn_disconnect").addEventListener("click", () => putus(false));
el("btn_clear").addEventListener("click", () => {
    logEl.innerHTML = "";
    log("Log dibersihkan", "info");
});

el("btn_start").addEventListener("click", () => {
    if (!armTopic) { log("Belum terhubung ke ROS2", "bad"); return; }
    armTopic.publish(new ROSLIB.Message({ data: true }));
    log("Perintah START dikirim", "ok");
});

el("btn_stop").addEventListener("click", () => {
    if (!armTopic) { log("Belum terhubung ke ROS2", "bad"); return; }
    armTopic.publish(new ROSLIB.Message({ data: false }));
    log("Perintah STOP dikirim", "warn");
});

// Spasi untuk START, S untuk STOP — meniru aba-aba wasit.
document.addEventListener("keydown", (e) => {
    const t = e.target.tagName;
    if (t === "INPUT" || t === "SELECT") { return; }
    if (e.code === "Space") {
        e.preventDefault();
        el("btn_start").click();
    }
    if (e.key === "s" || e.key === "S") {
        el("btn_stop").click();
    }
});

log("System initialized. Ready to connect.", "info");