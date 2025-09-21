
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

// int request_temp(void) {
//   memset((uint8_t *)PACKET_DATA_BUFFER, 0, PACKET_MAX_PAYLOAD_SIZE);
//   memset((uint8_t *)SPI_BUFFER, 0, SPI_BUFFER_SIZE);

//   dht_t temp_data = {0};
//   uint16_t data_length = 1;
//   header_t header = header_init(DIRECTION_MASTER_SLAVE, 0,
//   REQUEST_TEMPERATURE,
//                                 0, data_length);

//   packet_t packet = packet_init(header, PACKET_DATA_BUFFER, data_length);
//   return packet_send(packet, SPI_BUFFER);
// }

// int receive_temp() {
//   packet_t packet_recvd;
//   int bytes = packet_recv(&packet_recvd, SPI_BUFFER, 4 + sizeof(dht_t),
//   true); if (bytes > 0) {
//     printf("Packet received:\n");
//     packet_print(&packet_recvd, true);
//   }
//   return bytes;
// }

uint8_t HEADER_BUFFOR[PACKET_HEADER_SIZE] = {0};

int main() {

  hardware_init(0);
  dht_pin_init();

  sleep_ms(1000);

  printf("Master is up.\n");

  uint8_t master_write[BUFFER_SIZE] = {0xaa, 0xbb, 0xcc};
  uint8_t master_read[BUFFER_SIZE] = {0};

  sleep_ms(1000);

  dht_t data = {0};

  header_t recv_header = {0}, send_header = {0};

  send_header =
      header_init(DIRECTION_MASTER_SLAVE, 0, REQUEST_TEMPERATURE, 0, 4096);

  while (true) {

    printf("\nM:sending request header \n");
    header_print(send_header);
    header_send(send_header, HEADER_BUFFOR);
    sleep_ms(2000);
    printf("\nM:Waiting for response header\n");
    if (header_recv(&recv_header, HEADER_BUFFOR)) {
      printf("\nM:Received response header\n");
      header_print(recv_header);
    }

    sleep_ms(5000);
  }

  return 0;
}