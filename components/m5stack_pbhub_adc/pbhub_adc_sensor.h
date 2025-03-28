#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "porthub.h"

namespace esphome {
namespace m5stack_pbhub {

class PbHubADCSensor : public sensor::Sensor, public PollingComponent {
 public:
  PbHubADCSensor(PortHub *parent, uint8_t channel, uint8_t pin)
      : parent_(parent), channel_(channel), pin_(pin) {}

  void update() override {
    // Compute register address for the given channel/pin
    uint8_t reg = channel_ * 10 + pin_;  // Example encoding
    uint16_t raw = parent_->hub_a_read_value(reg);
    float voltage = (raw / 1023.0f) * 5.0f;  // scale to 0–5V
    this->publish_state(voltage);
  }

 protected:
  PortHub *parent_;
  uint8_t channel_;
  uint8_t pin_;
};

}  // namespace m5stack_pbhub
}  // namespace esphome
