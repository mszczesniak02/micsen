#include "dht.h"

void dht_pin_init(void) {
  gpio_init(DHT_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
}

uint8_t dht_init(void) {
  dht_rst();
  return dht_check();
}

uint8_t dht_read_bit(void) {
  uint8_t retry = 0;
  while (gpio_get(DHT_PIN) && retry < 100) { // wait become Low level
    retry++;
    sleep_us(1);
  }
  retry = 0;
  while (!gpio_get(DHT_PIN) && retry < 100) { // wait become High level
    retry++;
    sleep_us(1);
  }
  sleep_us(40); // wait 40us
  if (gpio_get(DHT_PIN))
    return 1;
  else
    return 0;
}

uint8_t dht_read_byte(void) {
  uint8_t dat = 0;

  for (uint8_t i = 0; i < 8; i++) {
    dat <<= 1;
    dat |= dht_read_bit();
  }
  return dat;
}

uint8_t dht_check(void) {
  uint8_t retry = 0;
  gpio_set_dir(DHT_PIN, GPIO_IN);            // SET INPUT
  while (gpio_get(DHT_PIN) && retry < 100) { // DHT11 Pull down 40~80us
    retry++;
    sleep_us(1);
  };
  if (retry >= 100)
    return 1;
  else
    retry = 0;
  while (!gpio_get(DHT_PIN) && retry < 100) { // DHT11 Pull up 40~80us
    retry++;
    sleep_us(1);
  };
  if (retry >= 100)
    return 1; // chack error
  return 0;
}

void dht_rst(void) {
  gpio_set_dir(DHT_PIN, GPIO_OUT); // SET OUTPUT
  gpio_put(DHT_PIN, 0);            // GPIOA.0=0
  sleep_ms(20);                    // Pull down Least 18ms
  gpio_put(DHT_PIN, 1);            // GPIOA.0=1
  sleep_us(30);                    // Pull up 20~40us
}

bool dht_read_data(dht_t *result) {
  uint8_t buf[5], i;
  dht_rst();
  if (dht_check() == 0) {
    for (i = 0; i < 5; i++) {
      buf[i] = dht_read_byte();
    }
    if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4]) {
      result->h_int = buf[0];
      result->h_dec = buf[1];
      result->t_int = buf[2];
      result->t_dec = buf[3];
      return 1;
    }
  }
  return 0;
}

void dht_print(dht_t *data) {
  printf("T: %u.%u°C | H: %u.%u%%\n", data->t_int, data->t_dec, data->h_int,
         data->h_dec);
}

bool dht_is_equal(const dht_t *a, const dht_t *b) {
  return (a->h_int == b->h_int && a->h_dec == b->h_dec &&
          a->t_int == b->t_int && a->t_dec == b->t_dec);
}
