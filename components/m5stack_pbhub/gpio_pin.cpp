// gpio_pin.cpp (registering virtual ADC pin)

#include "esphome/core/log.h"
#include "esphome/core/application.h"
#include "pbhub_analog_pin.h"

namespace esphome {
namespace m5stack_pbhub {

using namespace gpio;

AnalogInputPin *create_analog_input_pin(PortHub *hub, uint8_t channel, uint8_t pin) {
  return new PbHubAnalogInputPin(hub, channel, pin);
}

}  // namespace m5stack_pbhub
}  // namespace esphome
