// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from iris_msgs:msg/MotorCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/motor_command.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__BUILDER_HPP_
#define IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "iris_msgs/msg/detail/motor_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace iris_msgs
{

namespace msg
{

namespace builder
{

class Init_MotorCommand_enable
{
public:
  explicit Init_MotorCommand_enable(::iris_msgs::msg::MotorCommand & msg)
  : msg_(msg)
  {}
  ::iris_msgs::msg::MotorCommand enable(::iris_msgs::msg::MotorCommand::_enable_type arg)
  {
    msg_.enable = std::move(arg);
    return std::move(msg_);
  }

private:
  ::iris_msgs::msg::MotorCommand msg_;
};

class Init_MotorCommand_speed
{
public:
  explicit Init_MotorCommand_speed(::iris_msgs::msg::MotorCommand & msg)
  : msg_(msg)
  {}
  Init_MotorCommand_enable speed(::iris_msgs::msg::MotorCommand::_speed_type arg)
  {
    msg_.speed = std::move(arg);
    return Init_MotorCommand_enable(msg_);
  }

private:
  ::iris_msgs::msg::MotorCommand msg_;
};

class Init_MotorCommand_servo
{
public:
  explicit Init_MotorCommand_servo(::iris_msgs::msg::MotorCommand & msg)
  : msg_(msg)
  {}
  Init_MotorCommand_speed servo(::iris_msgs::msg::MotorCommand::_servo_type arg)
  {
    msg_.servo = std::move(arg);
    return Init_MotorCommand_speed(msg_);
  }

private:
  ::iris_msgs::msg::MotorCommand msg_;
};

class Init_MotorCommand_header
{
public:
  Init_MotorCommand_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotorCommand_servo header(::iris_msgs::msg::MotorCommand::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_MotorCommand_servo(msg_);
  }

private:
  ::iris_msgs::msg::MotorCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::iris_msgs::msg::MotorCommand>()
{
  return iris_msgs::msg::builder::Init_MotorCommand_header();
}

}  // namespace iris_msgs

#endif  // IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__BUILDER_HPP_
