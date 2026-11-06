// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from iris_msgs:msg/LaneState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/lane_state.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__LANE_STATE__STRUCT_HPP_
#define IRIS_MSGS__MSG__DETAIL__LANE_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__iris_msgs__msg__LaneState __attribute__((deprecated))
#else
# define DEPRECATED__iris_msgs__msg__LaneState __declspec(deprecated)
#endif

namespace iris_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct LaneState_
{
  using Type = LaneState_<ContainerAllocator>;

  explicit LaneState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->found = false;
      this->error = 0.0f;
      this->heading = 0.0f;
      this->n_blobs = 0l;
    }
  }

  explicit LaneState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->found = false;
      this->error = 0.0f;
      this->heading = 0.0f;
      this->n_blobs = 0l;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _found_type =
    bool;
  _found_type found;
  using _error_type =
    float;
  _error_type error;
  using _heading_type =
    float;
  _heading_type heading;
  using _n_blobs_type =
    int32_t;
  _n_blobs_type n_blobs;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__found(
    const bool & _arg)
  {
    this->found = _arg;
    return *this;
  }
  Type & set__error(
    const float & _arg)
  {
    this->error = _arg;
    return *this;
  }
  Type & set__heading(
    const float & _arg)
  {
    this->heading = _arg;
    return *this;
  }
  Type & set__n_blobs(
    const int32_t & _arg)
  {
    this->n_blobs = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    iris_msgs::msg::LaneState_<ContainerAllocator> *;
  using ConstRawPtr =
    const iris_msgs::msg::LaneState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<iris_msgs::msg::LaneState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<iris_msgs::msg::LaneState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      iris_msgs::msg::LaneState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<iris_msgs::msg::LaneState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      iris_msgs::msg::LaneState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<iris_msgs::msg::LaneState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<iris_msgs::msg::LaneState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<iris_msgs::msg::LaneState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__iris_msgs__msg__LaneState
    std::shared_ptr<iris_msgs::msg::LaneState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__iris_msgs__msg__LaneState
    std::shared_ptr<iris_msgs::msg::LaneState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const LaneState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->found != other.found) {
      return false;
    }
    if (this->error != other.error) {
      return false;
    }
    if (this->heading != other.heading) {
      return false;
    }
    if (this->n_blobs != other.n_blobs) {
      return false;
    }
    return true;
  }
  bool operator!=(const LaneState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct LaneState_

// alias to use template instance with default allocator
using LaneState =
  iris_msgs::msg::LaneState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace iris_msgs

#endif  // IRIS_MSGS__MSG__DETAIL__LANE_STATE__STRUCT_HPP_
