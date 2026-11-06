// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/lane_state.h"


#ifndef IRIS_MSGS__MSG__DETAIL__LANE_STATE__STRUCT_H_
#define IRIS_MSGS__MSG__DETAIL__LANE_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/LaneState in the package iris_msgs.
typedef struct iris_msgs__msg__LaneState
{
  std_msgs__msg__Header header;
  bool found;
  float error;
  float heading;
  int32_t n_blobs;
} iris_msgs__msg__LaneState;

// Struct for a sequence of iris_msgs__msg__LaneState.
typedef struct iris_msgs__msg__LaneState__Sequence
{
  iris_msgs__msg__LaneState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} iris_msgs__msg__LaneState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // IRIS_MSGS__MSG__DETAIL__LANE_STATE__STRUCT_H_
