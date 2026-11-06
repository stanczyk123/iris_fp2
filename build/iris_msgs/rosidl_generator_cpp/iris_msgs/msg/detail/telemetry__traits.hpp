// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from iris_msgs:msg/Telemetry.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/telemetry.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__TELEMETRY__TRAITS_HPP_
#define IRIS_MSGS__MSG__DETAIL__TELEMETRY__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "iris_msgs/msg/detail/telemetry__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace iris_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Telemetry & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: seq
  {
    out << "seq: ";
    rosidl_generator_traits::value_to_yaml(msg.seq, out);
    out << ", ";
  }

  // member: rpm
  {
    out << "rpm: ";
    rosidl_generator_traits::value_to_yaml(msg.rpm, out);
    out << ", ";
  }

  // member: enc
  {
    out << "enc: ";
    rosidl_generator_traits::value_to_yaml(msg.enc, out);
    out << ", ";
  }

  // member: vbat
  {
    out << "vbat: ";
    rosidl_generator_traits::value_to_yaml(msg.vbat, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Telemetry & msg,
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

  // member: seq
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "seq: ";
    rosidl_generator_traits::value_to_yaml(msg.seq, out);
    out << "\n";
  }

  // member: rpm
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "rpm: ";
    rosidl_generator_traits::value_to_yaml(msg.rpm, out);
    out << "\n";
  }

  // member: enc
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "enc: ";
    rosidl_generator_traits::value_to_yaml(msg.enc, out);
    out << "\n";
  }

  // member: vbat
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "vbat: ";
    rosidl_generator_traits::value_to_yaml(msg.vbat, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Telemetry & msg, bool use_flow_style = false)
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
  const iris_msgs::msg::Telemetry & msg,
  std::ostream & out, size_t indentation = 0)
{
  iris_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use iris_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const iris_msgs::msg::Telemetry & msg)
{
  return iris_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<iris_msgs::msg::Telemetry>()
{
  return "iris_msgs::msg::Telemetry";
}

template<>
inline const char * name<iris_msgs::msg::Telemetry>()
{
  return "iris_msgs/msg/Telemetry";
}

template<>
struct has_fixed_size<iris_msgs::msg::Telemetry>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<iris_msgs::msg::Telemetry>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<iris_msgs::msg::Telemetry>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IRIS_MSGS__MSG__DETAIL__TELEMETRY__TRAITS_HPP_
