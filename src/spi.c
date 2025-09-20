#include "spi.h"

void spi_slave_init(void) {
  // enable hardware spi
  spi_init(SPI_SLAVE, SPI_BAUD_RATE);
  spi_set_slave(SPI_SLAVE, true);
  // set pins directions
  gpio_set_function(SPI_SLAVE_MISO, GPIO_FUNC_SPI);
  gpio_set_function(SPI_SLAVE_SCLK, GPIO_FUNC_SPI);
  gpio_set_function(SPI_SLAVE_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(SPI_SLAVE_CS, GPIO_FUNC_SPI);
}

void spi_master_init(void) {
  // enable hardware spi
  spi_init(SPI_MASTER, SPI_BAUD_RATE);
  // set pins directions
  gpio_set_function(SPI_MASTER_MISO, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MASTER_SCLK, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MASTER_MOSI, GPIO_FUNC_SPI);
  gpio_set_function(SPI_MASTER_CS, GPIO_FUNC_SPI);
}

void spi_mode_init(bool is_slave) {
  if (is_slave) {
    spi_slave_init();
  } else {
    spi_master_init();
  }
}

int spi_send(uint8_t *buffer, size_t length) {
  return spi_write_blocking(SPI_ID, buffer, length);
};

int spi_recv(uint8_t *buffer, size_t length) {
  return spi_read_blocking(SPI_ID, 0, buffer, length);
};

void print_buffer(uint8_t *buffer, size_t len) {
  // size_t counter = 0;
  for (size_t i = 0; i < len; i++) {
    if (i % 3 == 0) {
      printf("\n");
    }
    printf("%02x ", buffer[i]);
  }
  printf("\n");
}