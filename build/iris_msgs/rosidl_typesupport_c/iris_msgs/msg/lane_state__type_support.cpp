// generated from rosidl_typesupport_c/resource/idl__type_support.cpp.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice

#include "cstddef"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "iris_msgs/msg/detail/lane_state__struct.h"
#include "iris_msgs/msg/detail/lane_state__type_support.h"
#include "iris_msgs/msg/detail/lane_state__functions.h"
#include "rosidl_typesupport_c/identifier.h"
#include "rosidl_typesupport_c/message_type_support_dispatch.h"
#include "rosidl_typesupport_c/type_support_map.h"
#include "rosidl_typesupport_c/visibility_control.h"
#include "rosidl_typesupport_interface/macros.h"

namespace iris_msgs
{

namespace msg
{

namespace rosidl_typesupport_c
{

typedef struct _LaneState_type_support_ids_t
{
  const char * typesupport_identifier[2];
} _LaneState_type_support_ids_t;

static const _LaneState_type_support_ids_t _LaneState_message_typesupport_ids = {
  {
    "rosidl_typesupport_fastrtps_c",  // ::rosidl_typesupport_fastrtps_c::typesupport_identifier,
    "rosidl_typesupport_introspection_c",  // ::rosidl_typesupport_introspection_c::typesupport_identifier,
  }
};

typedef struct _LaneState_type_support_symbol_names_t
{
  const char * symbol_name[2];
} _LaneState_type_support_symbol_names_t;

#define STRINGIFY_(s) #s
#define STRINGIFY(s) STRINGIFY_(s)

static const _LaneState_type_support_symbol_names_t _LaneState_message_typesupport_symbol_names = {
  {
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_c, iris_msgs, msg, LaneState)),
    STRINGIFY(ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, iris_msgs, msg, LaneState)),
  }
};

typedef struct _LaneState_type_support_data_t
{
  void * data[2];
} _LaneState_type_support_data_t;

static _LaneState_type_support_data_t _LaneState_message_typesupport_data = {
  {
    0,  // will store the shared library later
    0,  // will store the shared library later
  }
};

static const type_support_map_t _LaneState_message_typesupport_map = {
  2,
  "iris_msgs",
  &_LaneState_message_typesupport_ids.typesupport_identifier[0],
  &_LaneState_message_typesupport_symbol_names.symbol_name[0],
  &_LaneState_message_typesupport_data.data[0],
};

static const rosidl_message_type_support_t LaneState_message_type_support_handle = {
  rosidl_typesupport_c__typesupport_identifier,
  reinterpret_cast<const type_support_map_t *>(&_LaneState_message_typesupport_map),
  rosidl_typesupport_c__get_message_typesupport_handle_function,
  &iris_msgs__msg__LaneState__get_type_hash,
  &iris_msgs__msg__LaneState__get_type_description,
  &iris_msgs__msg__LaneState__get_type_description_sources,
};

}  // namespace rosidl_typesupport_c

}  // namespace msg

}  // namespace iris_msgs

#ifdef __cplusplus
extern "C"
{
#endif

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_c, iris_msgs, msg, LaneState)() {
  return &::iris_msgs::msg::rosidl_typesupport_c::LaneState_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
