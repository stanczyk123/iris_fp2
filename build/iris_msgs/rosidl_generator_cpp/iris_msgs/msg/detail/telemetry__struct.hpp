// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from iris_msgs:msg/Telemetry.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/telemetry.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__TELEMETRY__STRUCT_HPP_
#define IRIS_MSGS__MSG__DETAIL__TELEMETRY__STRUCT_HPP_

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
# define DEPRECATED__iris_msgs__msg__Telemetry __attribute__((deprecated))
#else
# define DEPRECATED__iris_msgs__msg__Telemetry __declspec(deprecated)
#endif

namespace iris_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Telemetry_
{
  using Type = Telemetry_<ContainerAllocator>;

  explicit Telemetry_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->seq = 0ul;
      this->rpm = 0.0f;
      this->enc = 0l;
      this->vbat = 0.0f;
    }
  }

  explicit Telemetry_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->seq = 0ul;
      this->rpm = 0.0f;
      this->enc = 0l;
      this->vbat = 0.0f;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _seq_type =
    uint32_t;
  _seq_type seq;
  using _rpm_type =
    float;
  _rpm_type rpm;
  using _enc_type =
    int32_t;
  _enc_type enc;
  using _vbat_type =
    float;
  _vbat_type vbat;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__seq(
    const uint32_t & _arg)
  {
    this->seq = _arg;
    return *this;
  }
  Type & set__rpm(
    const float & _arg)
  {
    this->rpm = _arg;
    return *this;
  }
  Type & set__enc(
    const int32_t & _arg)
  {
    this->enc = _arg;
    return *this;
  }
  Type & set__vbat(
    const float & _arg)
  {
    this->vbat = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    iris_msgs::msg::Telemetry_<ContainerAllocator> *;
  using ConstRawPtr =
    const iris_msgs::msg::Telemetry_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      iris_msgs::msg::Telemetry_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      iris_msgs::msg::Telemetry_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__iris_msgs__msg__Telemetry
    std::shared_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__iris_msgs__msg__Telemetry
    std::shared_ptr<iris_msgs::msg::Telemetry_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Telemetry_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->seq != other.seq) {
      return false;
    }
    if (this->rpm != other.rpm) {
      return false;
    }
    if (this->enc != other.enc) {
      return false;
    }
    if (this->vbat != other.vbat) {
      return false;
    }
    return true;
  }
  bool operator!=(const Telemetry_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Telemetry_

// alias to use template instance with default allocator
using Telemetry =
  iris_msgs::msg::Telemetry_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace iris_msgs

#endif  // IRIS_MSGS__MSG__DETAIL__TELEMETRY__STRUCT_HPP_
