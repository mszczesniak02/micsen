#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "hardware/spi.h"
#include "pico/stdlib.h"

#include "config.h"
#include "packet_types.h"
#include "spi.h"
#include "sta.h"

/**
 * @brief Defines the header for an SPI packet using a union.
 *
 * This union allows accessing the header data in two ways:
 * 1. As a single 32-bit integer (as_u32) for easy transmission.
 * 2. As a structured set of bit-fields (fields) for easy access to
 * individual flags. The total size is 4 bytes.
 */

//   goal -> comunication in packets
//   directions -> master <-> slave
//                terminal <-> master
//                4 bits
// request & responce packet for master - slave,
//                up to 4096 packets mic data, as fast as possible to send via
//                BLE.

typedef union {
  uint32_t as_u32;
  struct __attribute__((packed)) {
    uint8_t starting_sequence : 3; // Starting sequence: 0b 111
    uint8_t remaining_packets : 5; // 5 bits for setting remaining packets to
                                   // get all the data. Packet buffer is 4096,
                                   // max one packet data size is 2^16-1, so +-
                                   // 16 packets, 5 bits neceserry

    uint8_t packet_direction : 4; // 0: STA->Master, 1: MASTER->STA, 2:
                                  // MASTER->SLAVE, 3: SLAVE->MASTER
    uint8_t packet_type : 4; // 0: MICROPHONE REQUEST, 1: MICROPHONE RESPONCE,
                             // 2: TEMPERATURE REQUEST, 3: TEMPERATURE RESPONSE
    uint16_t
        payload_or_request_size; // 2^16-1 max payload,
                                 //  if response->payload size, if request->
                                 // request (microphone) measurements
                                 // amount
  } fields;
} header_t;

/**
 * @brief Defines the complete structure for an SPI packet.
 *
 * This structure combines the header, payload, and checksum (CRC)
 * into a single, coherent packet ready for transmission.
 */
typedef struct __attribute__((packed)) {
  header_t header; // The header with packet information.
  uint8_t payload[PACKET_MAX_PAYLOAD_SIZE]; // Buffer for the data to be sent.
} packet_t;

void header_set(header_t *header, uint32_t fields);

header_t header_init(uint8_t direction, uint8_t is_response,
                     uint8_t packet_type, uint8_t multiple_packet_amount,
                     uint16_t data_size);

void packet_set(packet_t *packet, uint8_t *buffer, size_t buffer_len,
                uint32_t header);

packet_t packet_init(header_t header, uint8_t *buffer, size_t length);
/**
 * @brief Deserializes a byte buffer into a packet_t structure.
 *
 * @param buffer Pointer to the source byte buffer.
 * @param packet Pointer to the destination packet_t structure.
 */
size_t packet_serialize(packet_t *packet, uint8_t *buffer);

/**
 * @brief Deserializes a byte buffer into a packet_t structure, byte by byte.
 * @param buffer Pointer to the source buffer.
 * @param packet Pointer to the destination packet structure.
 * @return The total number of bytes read from the buffer.
 */
size_t packet_deserialize(packet_t *packet, uint8_t *buffer);

/**
 * @brief Print the packet
 * @param packet packet to read.
 * @param as_hex show the payload as hex, else -> print as chars.
 */
void packet_print(packet_t *packet, bool as_hex);

int packet_send(packet_t packet, uint8_t *send_buffer);

int packet_recv(packet_t *packet, uint8_t *recv_buffer, size_t length);
