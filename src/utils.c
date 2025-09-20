
#include "utils.h"

void set_led(bool enable) {
  if (enable) {
    gpio_put(PICO_DEFAULT_LED_PIN, 1); // LED ON

  } else {
    gpio_put(PICO_DEFAULT_LED_PIN, 0); // LED ON
  }
}

void init_led(void) {
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_set_dir(25, GPIO_OUT);
}

void hardware_init(bool is_slave) {
  stdio_init_all();
  init_led();
  spi_mode_init(is_slave);
}
