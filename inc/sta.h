#pragma once

#include "config.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

void sta_init(void);

void sta_send(uint8_t *buffer, size_t length);

void sta_recv(uint8_t *buffer, size_t length);
