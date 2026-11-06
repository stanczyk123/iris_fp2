#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "iris_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__iris_msgs__msg__LaneState() -> *const std::ffi::c_void;
}

#[link(name = "iris_msgs__rosidl_generator_c")]
extern "C" {
    fn iris_msgs__msg__LaneState__init(msg: *mut LaneState) -> bool;
    fn iris_msgs__msg__LaneState__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<LaneState>, size: usize) -> bool;
    fn iris_msgs__msg__LaneState__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<LaneState>);
    fn iris_msgs__msg__LaneState__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<LaneState>, out_seq: *mut rosidl_runtime_rs::Sequence<LaneState>) -> bool;
}

// Corresponds to iris_msgs__msg__LaneState
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct LaneState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,


    // This member is not documented.
    #[allow(missing_docs)]
    pub found: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub error: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub heading: f32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub n_blobs: i32,

}



impl Default for LaneState {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !iris_msgs__msg__LaneState__init(&mut msg as *mut _) {
        panic!("Call to iris_msgs__msg__LaneState__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for LaneState {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__LaneState__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__LaneState__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__LaneState__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for LaneState {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for LaneState where Self: Sized {
  const TYPE_NAME: &'static str = "iris_msgs/msg/LaneState";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__iris_msgs__msg__LaneState() }
  }
}


#[link(name = "iris_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__iris_msgs__msg__MotorCommand() -> *const std::ffi::c_void;
}

#[link(name = "iris_msgs__rosidl_generator_c")]
extern "C" {
    fn iris_msgs__msg__MotorCommand__init(msg: *mut MotorCommand) -> bool;
    fn iris_msgs__msg__MotorCommand__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MotorCommand>, size: usize) -> bool;
    fn iris_msgs__msg__MotorCommand__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MotorCommand>);
    fn iris_msgs__msg__MotorCommand__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MotorCommand>, out_seq: *mut rosidl_runtime_rs::Sequence<MotorCommand>) -> bool;
}

// Corresponds to iris_msgs__msg__MotorCommand
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MotorCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// sudut kemudi derajat, negatif = kiri
    pub servo: f32,

    /// 0-255, diteruskan ESP32 ke STM32
    pub speed: i32,

    /// false = motor berhenti
    pub enable: bool,

}



impl Default for MotorCommand {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !iris_msgs__msg__MotorCommand__init(&mut msg as *mut _) {
        panic!("Call to iris_msgs__msg__MotorCommand__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MotorCommand {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__MotorCommand__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__MotorCommand__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__MotorCommand__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MotorCommand {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MotorCommand where Self: Sized {
  const TYPE_NAME: &'static str = "iris_msgs/msg/MotorCommand";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__iris_msgs__msg__MotorCommand() }
  }
}


#[link(name = "iris_msgs__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__iris_msgs__msg__Telemetry() -> *const std::ffi::c_void;
}

#[link(name = "iris_msgs__rosidl_generator_c")]
extern "C" {
    fn iris_msgs__msg__Telemetry__init(msg: *mut Telemetry) -> bool;
    fn iris_msgs__msg__Telemetry__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Telemetry>, size: usize) -> bool;
    fn iris_msgs__msg__Telemetry__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Telemetry>);
    fn iris_msgs__msg__Telemetry__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Telemetry>, out_seq: *mut rosidl_runtime_rs::Sequence<Telemetry>) -> bool;
}

// Corresponds to iris_msgs__msg__Telemetry
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Telemetry {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

    /// nomor urut yang dipantulkan ESP32, untuk ukur loss & RTT
    pub seq: u32,


    // This member is not documented.
    #[allow(missing_docs)]
    pub rpm: f32,

    /// pembacaan encoder kumulatif
    pub enc: i32,

    /// tegangan baterai, Volt
    pub vbat: f32,

}



impl Default for Telemetry {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !iris_msgs__msg__Telemetry__init(&mut msg as *mut _) {
        panic!("Call to iris_msgs__msg__Telemetry__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Telemetry {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__Telemetry__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__Telemetry__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { iris_msgs__msg__Telemetry__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Telemetry {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Telemetry where Self: Sized {
  const TYPE_NAME: &'static str = "iris_msgs/msg/Telemetry";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__iris_msgs__msg__Telemetry() }
  }
}


