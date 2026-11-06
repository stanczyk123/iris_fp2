# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target iris_msgs::iris_msgs
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${iris_msgs_TARGETS}.
if(iris_msgs_TARGETS AND NOT TARGET iris_msgs::iris_msgs)
  add_library(iris_msgs::iris_msgs INTERFACE IMPORTED)
  set_target_properties(iris_msgs::iris_msgs PROPERTIES
    INTERFACE_LINK_LIBRARIES "${iris_msgs_TARGETS}")
endif()
