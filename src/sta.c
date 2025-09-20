#include "sta.h"

void sta_init(void) {

  gpio_set_function(STA_UART_RX_PIN,
                    UART_FUNCSEL_NUM(STA_UART_ID, STA_UART_RX_PIN));
  gpio_set_function(STA_UART_TX_PIN,
                    UART_FUNCSEL_NUM(STA_UART_ID, STA_UART_TX_PIN));

  uart_init(STA_UART_ID, STA_UART_BAUDRATE);
}

void sta_send(uint8_t *buffer, size_t length) {
  uart_write_blocking(STA_UART_ID, buffer, length);
}

void sta_recv(uint8_t *buffer, size_t length) {
  uart_read_blocking(STA_UART_ID, buffer, length);
}
