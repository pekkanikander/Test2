// Wokwi ST1VAFE3BX biosensor
//
// SPDX-License-Identifier: MIT
// Copyright 2025 Pekka-Ilmari Nikander <pekka.nikander@iki.fi>

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

const uint8_t CHIP_WHO_AM_I = 0x48;

// NOTE! Currently only I2C is supported. Support for SPI and I3C are TBD. The CS pin is ignored.

static const int I2C_ADDRESS_BASE = 0x18;

typedef struct {
  pin_t TA0;
  bool initialized;
} chip_state_t;

static bool on_i2c_connect(void *user_data, uint32_t address, bool read);
static uint8_t on_i2c_read(void *user_data);
static bool on_i2c_write(void *user_data, uint8_t data);

void chip_init() {
  chip_state_t chip = {
    .TA0 = pin_init("SDO/TA0", INPUT),
  };

  i2c_config_t i2c = {
    .address = I2C_ADDRESS_BASE,
    .scl = pin_init("SCL/SPC", INPUT_PULLUP),
    .sda = pin_init("SDA/SDI/SDO", INPUT_PULLUP),
    .connect = on_i2c_connect,
    .read = on_i2c_read,
    .write = on_i2c_write,
    .disconnect = NULL,
    .user_data = &chip,
  };

  printf("Hello from ST1VAFE3BX!\n");

  // I2C Configuration
  if (pin_read(chip.TA0)) {
    i2c.address += 1;
  }
  i2c_init(&i2c);
}

static bool on_i2c_connect(void *user_data, uint32_t address, bool read) {
  chip_state_t *const chip = user_data;

  if (!chip->initialized) {
    chip->initialized = 1;
    return false;
  }

  return true;
};

static uint8_t on_i2c_read(void *user_data) {
  return CHIP_WHO_AM_I; 
}

static bool on_i2c_write(void *user_data, uint8_t data) {
  return true;
}