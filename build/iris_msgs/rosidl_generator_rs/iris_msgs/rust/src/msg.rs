#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to iris_msgs__msg__LaneState

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct LaneState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,


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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::LaneState::default())
  }
}

impl rosidl_runtime_rs::Message for LaneState {
  type RmwMsg = super::msg::rmw::LaneState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        found: msg.found,
        error: msg.error,
        heading: msg.heading,
        n_blobs: msg.n_blobs,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      found: msg.found,
      error: msg.error,
      heading: msg.heading,
      n_blobs: msg.n_blobs,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      found: msg.found,
      error: msg.error,
      heading: msg.heading,
      n_blobs: msg.n_blobs,
    }
  }
}


// Corresponds to iris_msgs__msg__MotorCommand

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MotorCommand {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// sudut kemudi derajat, negatif = kiri
    pub servo: f32,

    /// 0-255, diteruskan ESP32 ke STM32
    pub speed: i32,

    /// false = motor berhenti
    pub enable: bool,

}



impl Default for MotorCommand {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::MotorCommand::default())
  }
}

impl rosidl_runtime_rs::Message for MotorCommand {
  type RmwMsg = super::msg::rmw::MotorCommand;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        servo: msg.servo,
        speed: msg.speed,
        enable: msg.enable,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      servo: msg.servo,
      speed: msg.speed,
      enable: msg.enable,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      servo: msg.servo,
      speed: msg.speed,
      enable: msg.enable,
    }
  }
}


// Corresponds to iris_msgs__msg__Telemetry

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Telemetry {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

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
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Telemetry::default())
  }
}

impl rosidl_runtime_rs::Message for Telemetry {
  type RmwMsg = super::msg::rmw::Telemetry;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        seq: msg.seq,
        rpm: msg.rpm,
        enc: msg.enc,
        vbat: msg.vbat,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      seq: msg.seq,
      rpm: msg.rpm,
      enc: msg.enc,
      vbat: msg.vbat,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      seq: msg.seq,
      rpm: msg.rpm,
      enc: msg.enc,
      vbat: msg.vbat,
    }
  }
}


