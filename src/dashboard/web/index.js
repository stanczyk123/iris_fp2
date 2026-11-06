// Slideshow layar sambutan.
// Berganti tiap 4 detik dengan pudar silang.

const JEDA = 4000;

const slides = document.querySelectorAll(".slide");
const wadahTitik = document.getElementById("titik");

let aktif = 0;
let timer = null;

// Bikin satu titik penanda untuk tiap foto, supaya bisa diklik juga.
slides.forEach((_, i) => {
    const t = document.createElement("span");
    if (i === 0) { t.classList.add("tampil"); }
    t.addEventListener("click", () => {
        tampilkan(i);
        mulaiTimer();     // ulang hitungan, supaya tidak langsung berganti
    });
    wadahTitik.appendChild(t);
});

const titik = wadahTitik.querySelectorAll("span");

function tampilkan(i) {
    slides[aktif].classList.remove("tampil");
    titik[aktif].classList.remove("tampil");
    aktif = i;
    slides[aktif].classList.add("tampil");
    titik[aktif].classList.add("tampil");
}

function berikutnya() {
    tampilkan((aktif + 1) % slides.length);
}

function mulaiTimer() {
    clearInterval(timer);
    timer = setInterval(berikutnya, JEDA);
}

mulaiTimer();

// Hentikan pergantian saat tab tidak terlihat, lalu lanjutkan lagi.
// Tanpa ini, kembali ke tab bisa memicu beberapa pergantian sekaligus.
document.addEventListener("visibilitychange", () => {
    if (document.hidden) {
        clearInterval(timer);
    } else {
        mulaiTimer();
    }
});