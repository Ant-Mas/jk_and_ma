#pragma once

#include <stdint.h>

#define ADC_PIN 26
#define MOVING_AVG_SIZE 16

uint8_t map(uint16_t x);

void init_adc(void);

uint8_t scan_adc(void);