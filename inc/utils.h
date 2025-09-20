#pragma once

#include "hardware/spi.h"
#include "pico/stdlib.h"

#include <stdbool.h>
#include <stdio.h>

#include "config.h"
#include "spi.h"

void set_led(bool enable);

void init_led(void);

void hardware_init(bool is_slave);
