cmake_minimum_required(VERSION 3.8)
project(master)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(iris_msgs REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(master_node src/master_node.cpp)
ament_target_dependencies(master_node rclcpp iris_msgs std_msgs)

install(TARGETS master_node
  DESTINATION lib/${PROJECT_NAME}
  
)


install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)
ament_package()