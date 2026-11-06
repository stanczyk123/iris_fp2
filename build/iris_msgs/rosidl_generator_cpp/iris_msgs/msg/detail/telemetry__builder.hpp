// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from iris_msgs:msg/Telemetry.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/telemetry.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__TELEMETRY__BUILDER_HPP_
#define IRIS_MSGS__MSG__DETAIL__TELEMETRY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "iris_msgs/msg/detail/telemetry__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace iris_msgs
{

namespace msg
{

namespace builder
{

class Init_Telemetry_vbat
{
public:
  explicit Init_Telemetry_vbat(::iris_msgs::msg::Telemetry & msg)
  : msg_(msg)
  {}
  ::iris_msgs::msg::Telemetry vbat(::iris_msgs::msg::Telemetry::_vbat_type arg)
  {
    msg_.vbat = std::move(arg);
    return std::move(msg_);
  }

private:
  ::iris_msgs::msg::Telemetry msg_;
};

class Init_Telemetry_enc
{
public:
  explicit Init_Telemetry_enc(::iris_msgs::msg::Telemetry & msg)
  : msg_(msg)
  {}
  Init_Telemetry_vbat enc(::iris_msgs::msg::Telemetry::_enc_type arg)
  {
    msg_.enc = std::move(arg);
    return Init_Telemetry_vbat(msg_);
  }

private:
  ::iris_msgs::msg::Telemetry msg_;
};

class Init_Telemetry_rpm
{
public:
  explicit Init_Telemetry_rpm(::iris_msgs::msg::Telemetry & msg)
  : msg_(msg)
  {}
  Init_Telemetry_enc rpm(::iris_msgs::msg::Telemetry::_rpm_type arg)
  {
    msg_.rpm = std::move(arg);
    return Init_Telemetry_enc(msg_);
  }

private:
  ::iris_msgs::msg::Telemetry msg_;
};

class Init_Telemetry_seq
{
public:
  explicit Init_Telemetry_seq(::iris_msgs::msg::Telemetry & msg)
  : msg_(msg)
  {}
  Init_Telemetry_rpm seq(::iris_msgs::msg::Telemetry::_seq_type arg)
  {
    msg_.seq = std::move(arg);
    return Init_Telemetry_rpm(msg_);
  }

private:
  ::iris_msgs::msg::Telemetry msg_;
};

class Init_Telemetry_header
{
public:
  Init_Telemetry_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Telemetry_seq header(::iris_msgs::msg::Telemetry::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Telemetry_seq(msg_);
  }

private:
  ::iris_msgs::msg::Telemetry msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::iris_msgs::msg::Telemetry>()
{
  return iris_msgs::msg::builder::Init_Telemetry_header();
}

}  // namespace iris_msgs

#endif  // IRIS_MSGS__MSG__DETAIL__TELEMETRY__BUILDER_HPP_
