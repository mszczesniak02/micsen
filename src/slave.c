#include "hardware/spi.h"
#include "pico/stdlib.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include "dht.h"
#include "packet.h"
#include "sta.h"
#include "utils.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "ky037.h"
#include "task.h"

TaskHandle_t dht_read_handle = NULL;
TaskHandle_t mic_read_handle = NULL;

/**
 * @brief read dht11 sensor data, print output every 2s.
 * If data read was the same as the previous measurement, do not print the data
 * @param arg dht_t data structure to write
 */
void dht_read_task(void *arg) {
  uint32_t counter = 0;

  dht_t *data = (dht_t *)arg;
  dht_t data_previous = {0};

  while (true) {
    printf("Task:(%u)\t", counter++);
    if (dht_read_data(data)) {
      if (!dht_is_equal(&data_previous, data)) {
        dht_print(data);
        data_previous = *data;
      }
    }
    vTaskDelay(2000);
    printf("\n");
  }
}

#define MIC_MEASUREMENTS 255

void mic_read_task(void *arg) {
  mic_t measurement[MIC_MEASUREMENTS] = {0};

  while (true) {
    vTaskDelay(1000);
    printf("Measuring microphone in 3 \n");
    vTaskDelay(1000);
    printf("Measuring microphone in 2 \n");
    vTaskDelay(1000);
    printf("Measuring microphone in 1 \n");
    vTaskDelay(1000);
    printf("NOW!\n");

    for (uint8_t i = 0; i < MIC_MEASUREMENTS; i++) {
      measurement[i] = mic_read();
    }
    printf("Measuring end.\n");
    mic_print(measurement, MIC_MEASUREMENTS, false);
  }
}

// uint8_t PACKET_DATA_BUFFER[PACKET_MAX_PAYLOAD_SIZE];
// uint8_t SPI_BUFFER[SPI_BUFFER_SIZE];

// int response_temp(void) {
//   memset((uint8_t *)PACKET_DATA_BUFFER, 0, PACKET_MAX_PAYLOAD_SIZE);
//   memset((uint8_t *)SPI_BUFFER, 0, SPI_BUFFER_SIZE);

//   packet_t packet_reader, packet_sender;
//   int bytes = packet_recv(&packet_reader, SPI_BUFFER, 4 + 1, true);

//   if (bytes <= 0) {
//     return 0; // Nie ma żądania, wyjdź
//   }

//   // Sprawdź, czy to faktycznie żądanie temperatury
//   if (packet_reader.header.fields.packet_type == REQUEST_TEMPERATURE) {
//     dht_t temp_data = {0};
//     dht_read_data(&temp_data); // Użyj poprawnej funkcji

//     // Skopiuj dane do bufora
//     memcpy(PACKET_DATA_BUFFER, &temp_data, sizeof(dht_t));

//     header_t header_sender = header_init(
//         DIRECTION_SLAVE_MASTER, 1, RESPONSE_TEMPERATURE, 0, sizeof(dht_t));
//     packet_sender =
//         packet_init(header_sender, PACKET_DATA_BUFFER, sizeof(dht_t));
//     return packet_send(packet_sender, SPI_BUFFER);
//   }
//   return 0;
// }
uint8_t HEADER_BUFFOR[PACKET_HEADER_SIZE] = {0};

int main() {

  hardware_init(1);
  dht_pin_init();
  mic_pin_init();

  sleep_ms(1000);

  printf("Slave is up.!\n");

  uint8_t slave_write[BUFFER_SIZE] = {0x66, 0x55, 0x44};
  uint8_t slave_read[BUFFER_SIZE] = {0};

  dht_t data = {0};

  sleep_ms(1000);

  // xTaskCreate(dht_read_task, "dht_read task", 256, (void *)(&data), 1,
  //             &dht_read_handle);

  // xTaskCreate(mic_read_task, "mic_read task", 1024, (void *)1, 1,
  //             &mic_read_handle);

  // vTaskStartScheduler();

  header_t recv_header = {0}, send_header = {0};

  send_header =
      header_init(DIRECTION_SLAVE_MASTER, 1, RESPONSE_TEMPERATURE, 0, 2048);
  while (true) {

    printf("Waiting for request header\n");
    if (header_recv(&recv_header, HEADER_BUFFOR)) {
      printf("Received header request.\n");
      header_print(recv_header);

      printf("Sending response header \n");
      header_send(send_header, HEADER_BUFFOR);
    }

    sleep_ms(5000);
  }

  return 0;
}