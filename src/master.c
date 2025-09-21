
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "config.h"
#include "dht.h"
#include "packet.h"
#include "spi.h"
#include "sta.h"

#include "utils.h"

#define BUFFER_SIZE 3

uint8_t PACKET_DATA_BUFFER[PACKET_MAX_PAYLOAD_SIZE];

uint8_t SPI_BUFFER[SPI_BUFFER_SIZE];

int request_temp(void) {
  memset((uint8_t *)PACKET_DATA_BUFFER, 0, PACKET_MAX_PAYLOAD_SIZE);
  memset((uint8_t *)SPI_BUFFER, 0, SPI_BUFFER);

  dht_t temp_data = {0};
  uint16_t data_length = 1;
  header_t header = header_init(DIRECTION_MASTER_SLAVE, 0, REQUEST_TEMPERATURE,
                                0, data_length);

  packet_t packet = packet_init(header, PACKET_DATA_BUFFER, data_length);
  return packet_send(packet, SPI_BUFFER);
}

int receive_temp() {
  packet_t packet_recvd;
  int bytes = packet_recv(&packet_recvd, SPI_BUFFER, 4 + 4);
  printf("Packet received:\n");
  for (size_t i = 0; i < bytes; i++) {
    printf("%02x ", packet_recvd.payload[i]);
  }
  printf("\t end.\n");
  return bytes;
}

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

    printf("\nM:sending REQUEST \n");

    int bytes = request_temp();
    printf("\nM:Waiting for response\n");

    if (receive_temp()) {
      printf("Packet received");
    }

    sleep_ms(5000);

    // print_buffer(master_write, BUFFER_SIZE);

    // spi_write_read_blocking(SPI_MASTER, master_write, master_read,
    // BUFFER_SIZE);

    // printf("\nM:reading \n");
    // print_buffer(master_read, BUFFER_SIZE);

    // sleep_ms(2500);
  }

  return 0;
}