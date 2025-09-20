#include "hardware/spi.h"
#include "pico/stdlib.h"

#include <stdbool.h>
#include <stdio.h>

#include "config.h"
#include "dht.h"
#include "packet.h"
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

  xTaskCreate(dht_read_task, "dht_read task", 256, (void *)(&data), 1,
              &dht_read_handle);

  xTaskCreate(mic_read_task, "mic_read task", 1024, (void *)1, 1,
              &mic_read_handle);

  vTaskStartScheduler();

  while (true) {

    // printf("\nS:sending \n");
    // print_buffer(slave_write, BUFFER_SIZE);

    // spi_write_read_blocking(SPI_SLAVE, slave_write, slave_read, BUFFER_SIZE);

    // printf("\nS:reading \n");
    // print_buffer(slave_read, BUFFER_SIZE);

    // sleep_ms(2500);
  }

  return 0;
}