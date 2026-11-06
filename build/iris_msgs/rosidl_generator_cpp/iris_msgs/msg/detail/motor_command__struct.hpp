// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from iris_msgs:msg/MotorCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "iris_msgs/msg/motor_command.hpp"


#ifndef IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__STRUCT_HPP_
#define IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__STRUCT_HPP_

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
# define DEPRECATED__iris_msgs__msg__MotorCommand __attribute__((deprecated))
#else
# define DEPRECATED__iris_msgs__msg__MotorCommand __declspec(deprecated)
#endif

namespace iris_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct MotorCommand_
{
  using Type = MotorCommand_<ContainerAllocator>;

  explicit MotorCommand_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->servo = 0.0f;
      this->speed = 0l;
      this->enable = false;
    }
  }

  explicit MotorCommand_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->servo = 0.0f;
      this->speed = 0l;
      this->enable = false;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _servo_type =
    float;
  _servo_type servo;
  using _speed_type =
    int32_t;
  _speed_type speed;
  using _enable_type =
    bool;
  _enable_type enable;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__servo(
    const float & _arg)
  {
    this->servo = _arg;
    return *this;
  }
  Type & set__speed(
    const int32_t & _arg)
  {
    this->speed = _arg;
    return *this;
  }
  Type & set__enable(
    const bool & _arg)
  {
    this->enable = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    iris_msgs::msg::MotorCommand_<ContainerAllocator> *;
  using ConstRawPtr =
    const iris_msgs::msg::MotorCommand_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      iris_msgs::msg::MotorCommand_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      iris_msgs::msg::MotorCommand_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__iris_msgs__msg__MotorCommand
    std::shared_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__iris_msgs__msg__MotorCommand
    std::shared_ptr<iris_msgs::msg::MotorCommand_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const MotorCommand_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->servo != other.servo) {
      return false;
    }
    if (this->speed != other.speed) {
      return false;
    }
    if (this->enable != other.enable) {
      return false;
    }
    return true;
  }
  bool operator!=(const MotorCommand_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct MotorCommand_

// alias to use template instance with default allocator
using MotorCommand =
  iris_msgs::msg::MotorCommand_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace iris_msgs

#endif  // IRIS_MSGS__MSG__DETAIL__MOTOR_COMMAND__STRUCT_HPP_
