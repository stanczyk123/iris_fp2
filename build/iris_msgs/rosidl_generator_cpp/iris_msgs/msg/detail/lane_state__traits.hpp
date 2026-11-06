// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/lane_state.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__LANE_STATE__TRAITS_HPP_
#define IRIS_MSGS__MSG__DETAIL__LANE_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "iris_msgs/msg/detail/lane_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace iris_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const LaneState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: found
  {
    out << "found: ";
    rosidl_generator_traits::value_to_yaml(msg.found, out);
    out << ", ";
  }

  // member: error
  {
    out << "error: ";
    rosidl_generator_traits::value_to_yaml(msg.error, out);
    out << ", ";
  }

  // member: heading
  {
    out << "heading: ";
    rosidl_generator_traits::value_to_yaml(msg.heading, out);
    out << ", ";
  }

  // member: n_blobs
  {
    out << "n_blobs: ";
    rosidl_generator_traits::value_to_yaml(msg.n_blobs, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const LaneState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: found
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "found: ";
    rosidl_generator_traits::value_to_yaml(msg.found, out);
    out << "\n";
  }

  // member: error
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "error: ";
    rosidl_generator_traits::value_to_yaml(msg.error, out);
    out << "\n";
  }

  // member: heading
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "heading: ";
    rosidl_generator_traits::value_to_yaml(msg.heading, out);
    out << "\n";
  }

  // member: n_blobs
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "n_blobs: ";
    rosidl_generator_traits::value_to_yaml(msg.n_blobs, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const LaneState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace iris_msgs

namespace rosidl_generator_traits
{

[[deprecated("use iris_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const iris_msgs::msg::LaneState & msg,
  std::ostream & out, size_t indentation = 0)
{
  iris_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use iris_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const iris_msgs::msg::LaneState & msg)
{
  return iris_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<iris_msgs::msg::LaneState>()
{
  return "iris_msgs::msg::LaneState";
}

template<>
inline const char * name<iris_msgs::msg::LaneState>()
{
  return "iris_msgs/msg/LaneState";
}

template<>
struct has_fixed_size<iris_msgs::msg::LaneState>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<iris_msgs::msg::LaneState>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<iris_msgs::msg::LaneState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IRIS_MSGS__MSG__DETAIL__LANE_STATE__TRAITS_HPP_
