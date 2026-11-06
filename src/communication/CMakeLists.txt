cmake_minimum_required(VERSION 3.8)
project(communication)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(iris_msgs REQUIRED)

add_executable(udp_bridge_node src/udp_bridge_node.cpp)
ament_target_dependencies(udp_bridge_node rclcpp iris_msgs)

install(TARGETS udp_bridge_node
  DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS scripts/fake_esp32.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()