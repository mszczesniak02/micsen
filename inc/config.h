#pragma once
// THIS IS A CONFIG FILE OF AN ENTIRE PROJECT

// PACKET CONFIG
#define PACKET_MAX_PAYLOAD_SIZE 4096
#define BUFFER_SIZE 3

// PIN CONFIG:

#define LED_PIN PICO_DEFAULT_LED_PIN

// uart sta communication

#define STA_UART_ID uart0 // or uart 1
#define STA_UART_RX_PIN 0
#define STA_UART_TX_PIN 1
#define STA_UART_BAUDRATE 115200

// dht sensor
#define DHT_PIN 20
#define DHT_MAX_TIMINGS 85

// ky037 microphone
#define MIC_ID 0
#define MIC_PIN 26

// spi pins
#define SPI_MASTER_MOSI 19 // TX 25
#define SPI_MASTER_MISO 16 // RX 21
#define SPI_MASTER_SCLK 18 // SCLK 24
#define SPI_MASTER_CS 17   // CE 22

#define SPI_SLAVE_MOSI 0 // RX 16
#define SPI_SLAVE_SCLK 2 // SCK 9
#define SPI_SLAVE_CS 1   //  CE 17
#define SPI_SLAVE_MISO 3 // TX 6

#define SPI_ID spi0
#define SPI_MASTER spi0
#define SPI_SLAVE spi0

#define SPI_BAUD_RATE 500000
