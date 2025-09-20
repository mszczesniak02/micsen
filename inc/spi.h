#pragma once
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"

/**
 * @brief Sets the SPI peripheral to slave mode.
 */
void spi_slave_init(void);

/**
 * @brief Sets the SPI peripheral to master mode.
 */
void spi_master_init(void);

/**
 * @brief Set slave/master depending on variable
 * @param is_slave if true, set slave, else set master
 */
void spi_mode_init(bool is_slave);

void print_buffer(uint8_t *buffer, size_t len);

int spi_send(uint8_t *buffer, size_t length);

int spi_recv(uint8_t *buffer, size_t length);