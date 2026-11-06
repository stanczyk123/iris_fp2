// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "iris_msgs/msg/detail/lane_state__rosidl_typesupport_introspection_c.h"
#include "iris_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "iris_msgs/msg/detail/lane_state__functions.h"
#include "iris_msgs/msg/detail/lane_state__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  iris_msgs__msg__LaneState__init(message_memory);
}

void iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_fini_function(void * message_memory)
{
  iris_msgs__msg__LaneState__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_member_array[5] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(iris_msgs__msg__LaneState, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "found",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(iris_msgs__msg__LaneState, found),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "error",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(iris_msgs__msg__LaneState, error),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "heading",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(iris_msgs__msg__LaneState, heading),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "n_blobs",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(iris_msgs__msg__LaneState, n_blobs),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_members = {
  "iris_msgs__msg",  // message namespace
  "LaneState",  // message name
  5,  // number of fields
  sizeof(iris_msgs__msg__LaneState),
  false,  // has_any_key_member_
  iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_member_array,  // message members
  iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_init_function,  // function to initialize message memory (memory has to be allocated)
  iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_type_support_handle = {
  0,
  &iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_members,
  get_message_typesupport_handle_function,
  &iris_msgs__msg__LaneState__get_type_hash,
  &iris_msgs__msg__LaneState__get_type_description,
  &iris_msgs__msg__LaneState__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_iris_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, iris_msgs, msg, LaneState)() {
  iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_type_support_handle.typesupport_identifier) {
    iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &iris_msgs__msg__LaneState__rosidl_typesupport_introspection_c__LaneState_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
