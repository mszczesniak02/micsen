#pragma once

#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "pico/stdlib.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "config.h"

// read 12 bit ADC microphone data, remaining byte set to 0s.
typedef uint16_t mic_t;

// enable hardware
void mic_pin_init(void);

// read adc value
mic_t mic_read(void);

// print measured values
void mic_print(mic_t *value, size_t len, bool as_float);
