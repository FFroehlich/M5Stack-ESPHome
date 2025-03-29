// pbhub_analog_pin.h
#pragma once

#include "esphome/core/component.h"
#include "esphome/components/gpio/gpio.h"
#include "porthub.h"

namespace esphome {
namespace m5stack_pbhub {

class PbHubAnalogInputPin : public gpio::AnalogInputPin {
 public:
  PbHubAnalogInputPin(PortHub *hub, uint8_t channel, uint8_t pin)
      : hub_(hub), channel_(channel), pin_(pin) {}

  float analog_read() override {
    uint8_t reg = channel_ * 10 + pin_;  // simple register encoding
    uint16_t raw = hub_->hub_a_read_value(reg);
    return raw / 1023.0f * 3.3f;  // convert to volts (adjust for actual Vref)
  }

 protected:
  PortHub *hub_;
  uint8_t channel_;
  uint8_t pin_;
};

}  // namespace m5stack_pbhub
}  // namespace esphome
