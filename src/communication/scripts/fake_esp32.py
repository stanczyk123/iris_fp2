#!/usr/bin/env python3
"""ESP32 palsu, supaya udp_bridge_node bisa diuji sebelum ESP32 asli ada.

Menerima Command, mencetak sesekali, lalu membalas Telemetry dengan seq
yang sama supaya pengukuran packet loss bekerja.

Pakai:
    python3 firmware/fake_esp32.py

Lalu di terminal lain, dengan esp32_ip = 127.0.0.1:
    ros2 run communication udp_bridge_node
"""

import argparse
import json
import math
import socket
import time


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--listen", type=int, default=4210,
                    help="port masuk dari laptop")
    ap.add_argument("--reply-host", default="127.0.0.1")
    ap.add_argument("--reply-port", type=int, default=4211,
                    help="port telemetri ke laptop")
    ap.add_argument("--quiet", action="store_true")
    args, _ = ap.parse_known_args()   # abaikan argumen ROS2 seperti --ros-args

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("0.0.0.0", args.listen))
    sock.settimeout(1.0)

    print(f"[fake_esp32] dengar :{args.listen}, "
          f"balas ke {args.reply_host}:{args.reply_port}")
    print("[fake_esp32] Ctrl+C untuk berhenti\n")

    enc = 0
    count = 0
    t0 = time.monotonic()
    last_rx = None

    try:
        while True:
            try:
                raw, _ = sock.recvfrom(1024)
            except socket.timeout:
                # Meniru deadman timer yang nanti ada di firmware asli.
                if last_rx and time.monotonic() - last_rx > 0.5:
                    print("[fake_esp32] deadman aktif -> motor berhenti")
                    last_rx = None
                continue

            last_rx = time.monotonic()
            count += 1

            try:
                cmd = json.loads(raw.decode("utf-8"))
            except (UnicodeDecodeError, json.JSONDecodeError):
                print(f"[fake_esp32] paket rusak: {raw[:40]!r}")
                continue

            speed = int(cmd.get("speed", 0)) * int(cmd.get("en", 0))
            rpm = speed * 2.4 + 8 * math.sin(time.monotonic() * 3)
            enc += int(rpm / 60.0 * 20)
            vbat = max(3.2, 4.15 - 0.004 * (time.monotonic() - t0))

            # seq dipantulkan apa adanya. Inilah yang bikin laptop bisa
            # menghitung berapa paket hilang di perjalanan.
            sock.sendto(json.dumps({
                "seq": cmd.get("seq", 0),
                "rpm": round(rpm, 1),
                "enc": enc,
                "vbat": round(vbat, 3),
            }, separators=(",", ":")).encode("utf-8"),
                (args.reply_host, args.reply_port))

            if not args.quiet and count % 20 == 0:
                print(f"  rx#{cmd.get('seq')} servo={cmd.get('servo'):+6.2f} "
                      f"speed={cmd.get('speed'):3d} en={cmd.get('en')} "
                      f"-> rpm={rpm:.0f} vbat={vbat:.2f}")

    except KeyboardInterrupt:
        print(f"\n[fake_esp32] selesai, {count} paket diterima")
    finally:
        sock.close()


if __name__ == "__main__":
    main()