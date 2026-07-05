#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "m5stack_pbhub.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_pbhub {

static const char *const TAG = "m5stack_pbhub_sensor";

class PbHubADCSensor : public sensor::Sensor, public PollingComponent {
 public:
  PbHubADCSensor(M5StackPBHUBComponent *parent, uint8_t channel)
      : parent_(parent), channel_(channel) {}

  void update() override {
    uint16_t raw = this->parent_->analog_read(channel_);
    float voltage = raw / 4095.0f * 3.9f;
    ESP_LOGCONFIG(TAG, "PbHub analog read (channel %u): raw = %u / V = %f", channel_, raw, voltage);
    this->publish_state(voltage);
  }

 protected:
  M5StackPBHUBComponent *parent_;
  uint8_t channel_;
};

}  // namespace m5stack_pbhub_adc
}  // namespace esphome
