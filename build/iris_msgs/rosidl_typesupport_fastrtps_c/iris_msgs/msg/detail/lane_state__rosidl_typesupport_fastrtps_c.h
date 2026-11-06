// generated from rosidl_typesupport_fastrtps_c/resource/idl__rosidl_typesupport_fastrtps_c.h.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice
#ifndef IRIS_MSGS__MSG__DETAIL__LANE_STATE__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
#define IRIS_MSGS__MSG__DETAIL__LANE_STATE__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_


#include <stddef.h>
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "iris_msgs/msg/rosidl_typesupport_fastrtps_c__visibility_control.h"
#include "iris_msgs/msg/detail/lane_state__struct.h"
#include "fastcdr/Cdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
bool cdr_serialize_iris_msgs__msg__LaneState(
  const iris_msgs__msg__LaneState * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
bool cdr_deserialize_iris_msgs__msg__LaneState(
  eprosima::fastcdr::Cdr &,
  iris_msgs__msg__LaneState * ros_message);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
size_t get_serialized_size_iris_msgs__msg__LaneState(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
size_t max_serialized_size_iris_msgs__msg__LaneState(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
bool cdr_serialize_key_iris_msgs__msg__LaneState(
  const iris_msgs__msg__LaneState * ros_message,
  eprosima::fastcdr::Cdr & cdr);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
size_t get_serialized_size_key_iris_msgs__msg__LaneState(
  const void * untyped_ros_message,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
size_t max_serialized_size_key_iris_msgs__msg__LaneState(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

ROSIDL_TYPESUPPORT_FASTRTPS_C_PUBLIC_iris_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, iris_msgs, msg, LaneState)();

#ifdef __cplusplus
}
#endif

#endif  // IRIS_MSGS__MSG__DETAIL__LANE_STATE__ROSIDL_TYPESUPPORT_FASTRTPS_C_H_
