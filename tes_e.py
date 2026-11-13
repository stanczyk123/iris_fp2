#!/usr/bin/env python3
"""
tes_e.py — Tes E (daya tahan) IRIS CUP 2026, divisi programming.

Menerbitkan /robot/cmd 20 Hz tanpa putus selama tes berlangsung:
  speed : 300 rpm tetap
  servo : berganti +15 dan -15 derajat tiap 3 detik
  enable: true

Kenapa pakai skrip, bukan `ros2 topic pub` yang diganti-ganti manual:
setiap kali Ctrl+C lalu menjalankan ulang, aliran perintah putus beberapa
detik dan fail-safe STM32 ikut aktif. Baris FAIL-SAFE itu lalu mengotori
hasil tes, padahal yang sedang dicari justru fail-safe yang muncul SENDIRI.

Jalankan:
    cd ~/Desktop/iris_fp2 && source install/setup.bash
    python3 tes_e.py

Hentikan dengan Ctrl+C. Robot akan berhenti sendiri karena fail-safe
STM32 (0,5 detik tanpa perintah).

RODA HARUS TERANGKAT.
"""

import rclpy
from rclpy.node import Node
from iris_msgs.msg import MotorCommand

SPEED_RPM     = 300     # kecepatan tetap selama tes
SERVO_DEG     = 15.0    # besar sudut belok, bergantian + dan -
GANTI_DETIK   = 3.0     # berapa detik sekali servo berganti arah
LAPOR_DETIK   = 30      # berapa detik sekali mencetak laporan
HZ            = 20.0    # laju perintah, samakan dengan master_node


class TesE(Node):
    def __init__(self):
        super().__init__('tes_e')
        self.pub = self.create_publisher(MotorCommand, '/robot/cmd', 10)
        self.t0 = self.get_clock().now()
        self.blok_lapor = -1
        self.create_timer(1.0 / HZ, self.tick)
        self.get_logger().info(
            f'Tes E mulai: speed {SPEED_RPM} rpm, servo +/-{SERVO_DEG:.0f} '
            f'ganti tiap {GANTI_DETIK:.0f} detik. RODA HARUS TERANGKAT.')

    def tick(self):
        t = (self.get_clock().now() - self.t0).nanoseconds / 1e9

        m = MotorCommand()
        m.header.stamp = self.get_clock().now().to_msg()
        # Gelombang kotak: 0-3 s kanan, 3-6 s kiri, 6-9 s kanan, dan seterusnya.
        m.servo  = SERVO_DEG if int(t // GANTI_DETIK) % 2 == 0 else -SERVO_DEG
        m.speed  = SPEED_RPM
        m.enable = True
        self.pub.publish(m)

        blok = int(t) // LAPOR_DETIK
        if blok != self.blok_lapor:
            self.blok_lapor = blok
            menit, detik = divmod(int(t), 60)
            self.get_logger().info(
                f'{menit} menit {detik:02d} detik berjalan, servo {m.servo:+.0f} derajat')


def main():
    rclpy.init()
    node = TesE()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Tes E dihentikan. Robot berhenti lewat fail-safe STM32.')
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()