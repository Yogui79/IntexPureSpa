#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace spa {

class Spa : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_network_id(uint16_t network_id) { this->network_id_ = network_id; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_model(uint16_t model) { this->model_ = model; }

  void send_command(uint16_t command);
  void set_temperature(float temp);
  void set_filter_time(uint8_t hours);
  void set_sanitizer_time(uint8_t hours);

 protected:
  uint16_t network_id_;
  uint8_t channel_;
  uint16_t model_;

  // Add other variables from the original code
  bool first_send_ = true;
  uint16_t command_to_send_ = 0x0000;
  bool finish_pump_message_ = false;
  unsigned char data_[19];  // SIZE_PUMP_DATA +2
  uint16_t data_counter_ = 0;
  uint8_t state_ = 0;
  long last_time_receive_data_;
  bool error_communication_with_pump_ = false;

  // Commands
  static const uint16_t COMMAND_ON_OFF = 0x0001;
  static const uint16_t COMMAND_WATER_FILTER = 0x0010;
  static const uint16_t COMMAND_BUBBLE = 0x0020;
  static const uint16_t COMMAND_HEATER = 0x0040;
  static const uint16_t COMMAND_CELSIUS_FARENHEIT = 0x0002;
  static const uint16_t COMMAND_DECREASE = 0x0008;
  static const uint16_t COMMAND_INCREASE = 0x0004;
  static const uint16_t COMMAND_WATER_JET = 0x0100;
  static const uint16_t COMMAND_SANITIZER = 0x0080;

  // Status bytes
  static const uint8_t BYTE_STATUS_STATUS = 2;
  static const uint8_t BYTE_STATUS_COMMAND = 4;
  static const uint8_t BYTE_ACTUAL_TEMPERATURE = 5;
  static const uint8_t BYTE_SETPOINT_TEMPERATURE = 7;
  static const uint8_t BYTE_SETPOINT_TIME_SANITIZER = 8;
  static const uint8_t BYTE_SETPOINT_TIME_FILTER = 12;
  static const uint8_t BYTE_ERROR = 14;

  // Value bytes
  static const uint8_t VALUE_CONTROLLER_ON = 0x01;
  static const uint8_t VALUE_BUBBLE_ON = 0x10;
  static const uint8_t VALUE_HEATER_STANDBY = 0x02;
  static const uint8_t VALUE_HEATER_ON = 0x04;
  static const uint8_t VALUE_WATER_FILTER_ON = 0x08;
  static const uint8_t VALUE_WATER_JET_ON = 0x80;
  static const uint8_t VALUE_SANITIZER_ON = 0x20;

  void read_data(unsigned char c);
  void data_management();
  void send_life_fct();
  void set_settings(uint8_t channel);
  void send_command_management(uint16_t *command);
  void send_temperature_setpoint();
  void send_special_command(uint16_t command, bool *change_flag, bool switch_off, uint8_t target, uint8_t actual);

  uint8_t used_channel_;
  uint8_t first_command_char_;
};

template<typename... Ts> class SpaSendCommandAction : public Action<Ts...> {
 public:
  SpaSendCommandAction(Spa *parent) : parent_(parent) {}
  void set_parent(Spa *parent) { this->parent_ = parent; }
  TEMPLATABLE_VALUE(uint16_t, command)

  void play(Ts... x) override { this->parent_->send_command(this->command_.value(x...)); }

 protected:
  Spa *parent_;
};

template<typename... Ts> class SpaSetTemperatureAction : public Action<Ts...> {
 public:
  SpaSetTemperatureAction(Spa *parent) : parent_(parent) {}
  void set_parent(Spa *parent) { this->parent_ = parent; }
  TEMPLATABLE_VALUE(float, temperature)

  void play(Ts... x) override { this->parent_->set_temperature(this->temperature_.value(x...)); }

 protected:
  Spa *parent_;
};

template<typename... Ts> class SpaSetFilterTimeAction : public Action<Ts...> {
 public:
  SpaSetFilterTimeAction(Spa *parent) : parent_(parent) {}
  void set_parent(Spa *parent) { this->parent_ = parent; }
  TEMPLATABLE_VALUE(uint8_t, hours)

  void play(Ts... x) override { this->parent_->set_filter_time(this->hours_.value(x...)); }

 protected:
  Spa *parent_;
};

template<typename... Ts> class SpaSetSanitizerTimeAction : public Action<Ts...> {
 public:
  SpaSetSanitizerTimeAction(Spa *parent) : parent_(parent) {}
  void set_parent(Spa *parent) { this->parent_ = parent; }
  TEMPLATABLE_VALUE(uint8_t, hours)

  void play(Ts... x) override { this->parent_->set_sanitizer_time(this->hours_.value(x...)); }

 protected:
  Spa *parent_;
};

}  // namespace spa
}  // namespace esphome