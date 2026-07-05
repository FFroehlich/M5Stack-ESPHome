#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/m5stack_pbhub/m5stack_pbhub.h"

namespace esphome {
namespace m5stack_pbhub {

class PbHubADCSensor : public sensor::Sensor, public PollingComponent {
 public:
  PbHubADCSensor(M5StackPBHUBComponent *parent, uint8_t channel, uint8_t pin)
      : parent_(parent), channel_(channel), pin_(pin) {}

  void update() override;
  void dump_config() override;

  void set_output_raw(bool raw) { this->raw_ = raw; }

 protected:
  M5StackPBHUBComponent *parent_;
  uint8_t channel_;
  uint8_t pin_;
  bool raw_{false};
};

}  // namespace m5stack_pbhub
}  // namespace esphome
