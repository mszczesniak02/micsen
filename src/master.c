
#include <stdbool.h>
#include <stdio.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "config.h"
#include "dht.h"
#include "packet.h"
#include "spi.h"
#include "utils.h"

#define BUFFER_SIZE 3

int main() {

  hardware_init(0);
  dht_pin_init();

  sleep_ms(1000);

  printf("Master is up.\n");

  uint8_t master_write[BUFFER_SIZE] = {0xaa, 0xbb, 0xcc};
  uint8_t master_read[BUFFER_SIZE] = {0};

  sleep_ms(1000);

  dht_t data = {0};

  while (true) {

    if (dht_read_data(&data)) {
      dht_print(&data);
    };

    // printf("\nM:sending \n");
    // print_buffer(master_write, BUFFER_SIZE);

    // spi_write_read_blocking(SPI_MASTER, master_write, master_read,
    // BUFFER_SIZE);

    // printf("\nM:reading \n");
    // print_buffer(master_read, BUFFER_SIZE);

    // sleep_ms(2500);
  }

  return 0;
}