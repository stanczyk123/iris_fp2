"""Menyalakan seluruh sistem IRIS CUP dengan satu perintah.

    ros2 launch master iris_cup.launch.py

Pilihan:
    source:=0                   pakai webcam laptop
    source:=/path/video.mp4     pakai rekaman
    use_fake_esp:=false         saat ESP32 asli sudah tersambung
    use_dashboard:=false        tanpa rosbridge, video server, dan web
    use_comm:=false             tanpa jembatan UDP sama sekali
"""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, ExecuteProcess,
                            IncludeLaunchDescription, OpaqueFunction)
from launch.launch_description_sources import AnyLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def launch_setup(context, *args, **kwargs):
    params = os.path.join(
        get_package_share_directory("master"), "config", "params.yaml")
    web_dir = os.path.join(
        get_package_share_directory("dashboard"), "web")

    source = LaunchConfiguration("source").perform(context)

    def dipakai(nama):
        return LaunchConfiguration(nama).perform(context) == "true"

    vision_params = [params]
    if source:
        vision_params.append({"source": source})

    aksi = [
        Node(
            package="vision",
            executable="vision_node",
            name="vision_node",
            output="screen",
            parameters=vision_params,
        ),
        Node(
            package="master",
            executable="master_node",
            name="master_node",
            output="screen",
            parameters=[params],
        ),
    ]

    if dipakai("use_comm"):
        aksi.append(Node(
            package="communication",
            executable="udp_bridge_node",
            name="udp_bridge_node",
            output="screen",
            parameters=[params],
        ))

    # ESP32 palsu. Matikan dengan use_fake_esp:=false begitu modul
    # aslinya tersambung, kalau tidak keduanya berebut port 4210.
    if dipakai("use_fake_esp"):
        aksi.append(Node(
            package="communication",
            executable="fake_esp32.py",
            name="fake_esp32",
            output="screen",
        ))

    if dipakai("use_dashboard"):
        # Jembatan topic ROS2 ke WebSocket untuk browser
        aksi.append(IncludeLaunchDescription(
            AnyLaunchDescriptionSource(os.path.join(
                get_package_share_directory("rosbridge_server"),
                "launch", "rosbridge_websocket_launch.xml"))))

        # Jembatan gambar ROS2 ke aliran MJPEG
        aksi.append(Node(
            package="web_video_server",
            executable="web_video_server",
            name="web_video_server",
            output="screen",
        ))

        # Penyaji halaman web
        aksi.append(ExecuteProcess(
            cmd=["python3", "-m", "http.server", "8000"],
            cwd=web_dir,
            output="screen",
            name="web_server",
        ))

    return aksi


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            "source", default_value="",
            description="Sumber kamera: '0' webcam, path video, atau URL"),
        DeclareLaunchArgument(
            "use_comm", default_value="true",
            description="Jalankan jembatan UDP ke ESP32"),
        DeclareLaunchArgument(
            "use_fake_esp", default_value="true",
            description="Jalankan ESP32 palsu"),
        DeclareLaunchArgument(
            "use_dashboard", default_value="true",
            description="Jalankan rosbridge, web_video_server, dan web"),
        OpaqueFunction(function=launch_setup),
    ])