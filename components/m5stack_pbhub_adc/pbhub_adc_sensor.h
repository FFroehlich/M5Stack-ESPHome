#pragma once

#include "esphome/core/component.h"
#include "esphome/components/m5stack_pbhub/porthub.h"

namespace esphome {
namespace m5stack_pbhub {

class PbHubADCSensor : public adc::ADCSensor, public PollingComponent {
 public:
  PbHubADCSensor(PortHub *parent, uint8_t channel, uint8_t pin)
      : parent_(parent), channel_(channel), pin_(pin) {}

 protected:
  PortHub *parent_;
  uint8_t channel_;
  uint8_t pin_;
};

}  // namespace m5stack_pbhub
}  // namespace esphome
