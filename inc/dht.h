#pragma once

#include "config.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include <math.h>
#include <stdio.h>

typedef struct {
  uint8_t h_int;
  uint8_t h_dec;
  uint8_t t_int;
  uint8_t t_dec;

} dht_t;
void dht_pin_init(void);

bool dht_read_data(dht_t *result);

uint8_t dht_init(void);

uint8_t dht_read_byte(void);

uint8_t dht_read_bit(void);

uint8_t dht_check(void);

void dht_rst(void);

void dht_print(dht_t *data);

bool dht_is_equal(const dht_t *a, const dht_t *b);
