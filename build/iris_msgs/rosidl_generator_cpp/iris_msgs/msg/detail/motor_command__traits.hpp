// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from iris_msgs:msg/MotorCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/motor_command.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__TRAITS_HPP_
#define IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "iris_msgs/msg/detail/motor_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace iris_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const MotorCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: servo
  {
    out << "servo: ";
    rosidl_generator_traits::value_to_yaml(msg.servo, out);
    out << ", ";
  }

  // member: speed
  {
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << ", ";
  }

  // member: enable
  {
    out << "enable: ";
    rosidl_generator_traits::value_to_yaml(msg.enable, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MotorCommand & msg,
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

  // member: servo
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "servo: ";
    rosidl_generator_traits::value_to_yaml(msg.servo, out);
    out << "\n";
  }

  // member: speed
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "speed: ";
    rosidl_generator_traits::value_to_yaml(msg.speed, out);
    out << "\n";
  }

  // member: enable
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "enable: ";
    rosidl_generator_traits::value_to_yaml(msg.enable, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MotorCommand & msg, bool use_flow_style = false)
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
  const iris_msgs::msg::MotorCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  iris_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use iris_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const iris_msgs::msg::MotorCommand & msg)
{
  return iris_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<iris_msgs::msg::MotorCommand>()
{
  return "iris_msgs::msg::MotorCommand";
}

template<>
inline const char * name<iris_msgs::msg::MotorCommand>()
{
  return "iris_msgs/msg/MotorCommand";
}

template<>
struct has_fixed_size<iris_msgs::msg::MotorCommand>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<iris_msgs::msg::MotorCommand>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<iris_msgs::msg::MotorCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__TRAITS_HPP_
