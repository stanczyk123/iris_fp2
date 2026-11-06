// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/lane_state.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__LANE_STATE__BUILDER_HPP_
#define IRIS_MSGS__MSG__DETAIL__LANE_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "iris_msgs/msg/detail/lane_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace iris_msgs
{

namespace msg
{

namespace builder
{

class Init_LaneState_n_blobs
{
public:
  explicit Init_LaneState_n_blobs(::iris_msgs::msg::LaneState & msg)
  : msg_(msg)
  {}
  ::iris_msgs::msg::LaneState n_blobs(::iris_msgs::msg::LaneState::_n_blobs_type arg)
  {
    msg_.n_blobs = std::move(arg);
    return std::move(msg_);
  }

private:
  ::iris_msgs::msg::LaneState msg_;
};

class Init_LaneState_heading
{
public:
  explicit Init_LaneState_heading(::iris_msgs::msg::LaneState & msg)
  : msg_(msg)
  {}
  Init_LaneState_n_blobs heading(::iris_msgs::msg::LaneState::_heading_type arg)
  {
    msg_.heading = std::move(arg);
    return Init_LaneState_n_blobs(msg_);
  }

private:
  ::iris_msgs::msg::LaneState msg_;
};

class Init_LaneState_error
{
public:
  explicit Init_LaneState_error(::iris_msgs::msg::LaneState & msg)
  : msg_(msg)
  {}
  Init_LaneState_heading error(::iris_msgs::msg::LaneState::_error_type arg)
  {
    msg_.error = std::move(arg);
    return Init_LaneState_heading(msg_);
  }

private:
  ::iris_msgs::msg::LaneState msg_;
};

class Init_LaneState_found
{
public:
  explicit Init_LaneState_found(::iris_msgs::msg::LaneState & msg)
  : msg_(msg)
  {}
  Init_LaneState_error found(::iris_msgs::msg::LaneState::_found_type arg)
  {
    msg_.found = std::move(arg);
    return Init_LaneState_error(msg_);
  }

private:
  ::iris_msgs::msg::LaneState msg_;
};

class Init_LaneState_header
{
public:
  Init_LaneState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_LaneState_found header(::iris_msgs::msg::LaneState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_LaneState_found(msg_);
  }

private:
  ::iris_msgs::msg::LaneState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::iris_msgs::msg::LaneState>()
{
  return iris_msgs::msg::builder::Init_LaneState_header();
}

}  // namespace iris_msgs

#endif  // IRIS_MSGS__MSG__DETAIL__LANE_STATE__BUILDER_HPP_
