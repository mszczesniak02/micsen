#include "ky037.h"

void mic_pin_init(void) {
  adc_init();
  adc_gpio_init(MIC_PIN);
  adc_select_input(MIC_ID);
}

mic_t mic_read(void) { return adc_read(); }

void mic_print(mic_t *value, size_t len, bool as_float) {
  uint8_t sqrt_len = (uint8_t)floor(sqrt(len));

  float coef = 3.3f / (1 << 12);

  if (len < 1) {
    printf("ADC read: Cannot read 0 bytes!\n");
    return;
  }

  if (as_float) {
    printf("ADC read %lu bytes (hex)\n", len);
    for (size_t i = 0; i < len; i++) {
      // if (value[i] != NULL) {

      if (i % (sqrt_len) == (sqrt_len - 3)) {
        printf("%04x\n", value[i]);
      } else {
        printf("%04x ", value[i]);
      }
      // }
    }
  } else {
    printf("ADC read %lu bytes [V])\n", len);
    for (size_t i = 0; i < len; i++) {
      // if (value[i] != NULL) {

      if (i % sqrt_len == sqrt_len - 3) {
        printf("%0.3f\n", ((float)value[i]) * coef);
      } else {
        printf("%0.3f ", ((float)value[i]) * coef);
      }
      // }
    }
  }
  printf("\n");
}