#include "porthub.h"
#include "esphome/core/log.h"

using esphome::i2c::ERROR_OK;
using esphome::i2c::I2CDevice;

static const char *const TAG = "m5stack_pbhub.porthub";

PortHub::PortHub() {}

PortHub::PortHub(I2CDevice *device) : device_(device) {}

// The PBHUB's STM32F030 firmware can't handle a combined write-then-read
// transaction (I2CDevice::read_bytes() issues a single transaction with a
// repeated START, which times out against this device) -- it needs the
// register-select write and the value read done as two separate
// transactions, each with its own STOP, exactly like the Arduino Wire
// beginTransmission()/endTransmission() + requestFrom() sequence this used
// to be.
uint16_t PortHub::hub_a_read_value(uint8_t reg) {
    uint8_t reg_byte = reg | 0x06;
    uint8_t data[2] = {0, 0};
    if (this->device_->write(&reg_byte, 1) != ERROR_OK || this->device_->read(data, 2) != ERROR_OK) {
      ESP_LOGW(TAG, "Analog read failed");
      return 0;
    }
    return (uint16_t(data[1]) << 8) | data[0];
}

uint8_t PortHub::hub_d_read_value_A(uint8_t reg) {
    uint8_t reg_byte = reg | 0x04;
    uint8_t data = 0;
    if (this->device_->write(&reg_byte, 1) != ERROR_OK || this->device_->read(&data, 1) != ERROR_OK) {
      ESP_LOGW(TAG, "Digital read A failed");
      return 0;
    }
    return data;
}

uint8_t PortHub::hub_d_read_value_B(uint8_t reg) {
    uint8_t reg_byte = reg | 0x05;
    uint8_t data = 0;
    if (this->device_->write(&reg_byte, 1) != ERROR_OK || this->device_->read(&data, 1) != ERROR_OK) {
      ESP_LOGW(TAG, "Digital read B failed");
      return 0;
    }
    return data;
}

void PortHub::hub_d_wire_value_A(uint8_t reg, uint16_t level) {
    uint8_t data = level & 0xff;
    this->device_->write_bytes(reg | 0x00, &data, 1);
}

void PortHub::hub_d_wire_value_B(uint8_t reg, uint16_t level) {
    uint8_t data = level & 0xff;
    this->device_->write_bytes(reg | 0x01, &data, 1);
}

void PortHub::hub_a_wire_value_A(uint8_t reg, uint16_t duty) {
    uint8_t data = duty & 0xff;
    this->device_->write_bytes(reg | 0x02, &data, 1);
}

void PortHub::hub_a_wire_value_B(uint8_t reg, uint16_t duty) {
    uint8_t data = duty & 0xff;
    this->device_->write_bytes(reg | 0x03, &data, 1);
}

void PortHub::hub_wire_length(uint8_t reg, uint16_t length) {
    uint8_t data[2] = {uint8_t(length & 0xff), uint8_t(length >> 8)};
    this->device_->write_bytes(reg | 0x08, data, 2);
}

void PortHub::hub_wire_index_color(uint8_t reg, uint16_t num, uint8_t r,
                                   int8_t g, uint8_t b) {
    uint8_t data[5] = {uint8_t(num & 0xff), uint8_t(num >> 8), r, uint8_t(g), b};
    this->device_->write_bytes(reg | 0x09, data, 5);
}

void PortHub::hub_wire_fill_color(uint8_t reg, uint16_t first, uint16_t count,
                                  uint8_t r, int8_t g, uint8_t b) {
    uint8_t data[7] = {uint8_t(first & 0xff), uint8_t(first >> 8),
                       uint8_t(count & 0xff), uint8_t(count >> 8),
                       r,                     uint8_t(g),
                       b};
    this->device_->write_bytes(reg | 0x0a, data, 7);
}

void PortHub::hub_wire_setBrightness(uint8_t reg, uint8_t brightness) {
    this->device_->write_bytes(reg | 0x0b, &brightness, 1);
}
