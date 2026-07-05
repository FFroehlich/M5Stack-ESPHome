#include "pbhub_adc_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5stack_pbhub {

static const char *const TAG = "m5stack_pbhub_adc";
// The PBHUB's STM32F030 exposes a 12 bit ADC referenced to 3.3V.
static const uint16_t PBHUB_ADC_MAX_VALUE = 4095;
static const float PBHUB_ADC_REFERENCE_VOLTAGE = 3.3f;

void PbHubADCSensor::update() {
  uint16_t raw = this->parent_->analog_read(this->channel_);
  if (this->raw_) {
    this->publish_state(raw);
  } else {
    this->publish_state(raw * PBHUB_ADC_REFERENCE_VOLTAGE / PBHUB_ADC_MAX_VALUE);
  }
}

void PbHubADCSensor::dump_config() {
  LOG_SENSOR("", "PBHUB ADC Sensor", this);
  ESP_LOGCONFIG(TAG, "  Channel: %u", this->channel_);
  ESP_LOGCONFIG(TAG, "  Pin: %u", this->pin_);
  LOG_UPDATE_INTERVAL(this);
}

}  // namespace m5stack_pbhub
}  // namespace esphome
