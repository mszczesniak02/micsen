#include "packet.h"

header_t header_init(void) {
  header_t h = {.fields.starting_sequence = 0b111,
                .fields.remaining_packets = 0,
                .fields.packet_direction = 0,
                .fields.packet_type = 0,
                .fields.payload_or_request_size = 0};
  return h;
}
void header_set(header_t *header, uint32_t fields) { header->as_u32 = fields; }

void packet_set(packet_t *packet, uint8_t *buffer, size_t buffer_len,
                uint32_t header) {
  packet->header.as_u32 = header;
  for (size_t i = 0; i < buffer_len; i++) {
    packet->payload[i] = buffer[i];
  }
}

size_t packet_serialize(packet_t *packet, uint8_t *buffer) {
  size_t counter = 0;

  uint32_t header_val = packet->header.as_u32;
  buffer[counter++] = (uint8_t)(header_val & 0xFF);
  buffer[counter++] = (uint8_t)((header_val >> 8) & 0xFF);
  buffer[counter++] = (uint8_t)((header_val >> 16) & 0xFF);
  buffer[counter++] = (uint8_t)((header_val >> 24) & 0xFF);

  for (size_t i = 0; i < packet->header.fields.payload_or_request_size; i++) {
    buffer[counter++] = packet->payload[i];
  }
  return counter;
}

size_t packet_deserialize(packet_t *packet, uint8_t *buffer) {
  size_t counter = 0;
  packet->header.as_u32 = (uint32_t)(buffer[counter]) |
                          ((uint32_t)(buffer[counter + 1]) << 8) |
                          ((uint32_t)(buffer[counter + 2]) << 16) |
                          ((uint32_t)(buffer[counter + 3]) << 24);
  counter += 4;

  for (size_t i = 0; i < packet->header.fields.payload_or_request_size; i++) {
    packet->payload[i] = buffer[counter++];
  }

  return counter;
}

void packet_print(packet_t *packet, bool as_hex) {
  printf("Header:start:rem_packets:direction:type:payload_size\n");
  printf("Header:%x:%x:%x:%x:%x\n", packet->header.fields.starting_sequence,
         packet->header.fields.remaining_packets,
         packet->header.fields.packet_direction,
         packet->header.fields.packet_type,
         packet->header.fields.payload_or_request_size);

  printf("Payload:\n");
  if (as_hex) {
    for (size_t i = 0; i < packet->header.fields.payload_or_request_size; i++) {
      if (i > 0 && i % 16 == 0) {
        printf("\n");
      }
      printf("%02x ", packet->payload[i]);
    }
    printf("\n");
  } else {
    printf("%.*s\n", (int)packet->header.fields.payload_or_request_size,
           packet->payload);
  }
}

int packet_send(packet_t packet, uint8_t *buffer) {
  int bytes = (int)packet_serialize(&packet, buffer);

  switch (packet.header.fields.packet_direction) {
  case DIRECTION_STA_MASTER: // UART // czu to zadziała ? doczytać !
  case DIRECTION_MASTER_STA: // UART
    sta_send(buffer, bytes);
    return bytes;
    break;
  case DIRECTION_MASTER_SLAVE: // SPI
  case DIRECTION_SLAVE_MASTER: // SPI
    return spi_send(buffer, bytes);
    break;
  default: /*INVALID PACKET DIRECTION*/
    break;
  };
}

int packet_recv(packet_t *packet, uint8_t *buffer, size_t length) {
  int bytes = 0;
  switch (packet->header.fields.packet_direction) {
  case DIRECTION_STA_MASTER: // UART
  case DIRECTION_MASTER_STA: // UART
    sta_recv(buffer, length);
    return (int)packet_deserialize(packet, buffer);
    break;
  case DIRECTION_MASTER_SLAVE: // SPI
  case DIRECTION_SLAVE_MASTER: // SPI
    bytes = spi_recv(buffer, length);
    return (int)packet_deserialize(packet, buffer);
    break;
  default: /*INVALID PACKET DIRECTION*/
    break;
  };
}

header_t header_init(uint8_t direction, uint8_t is_response,
                     uint8_t packet_type, uint8_t multiple_packet_amount,
                     uint16_t data) {

  header_t h;

  h.fields.starting_sequence = 0b111;
  if (multiple_packet_amount > 32) {
    // invalid packet
  } else {
    h.fields.remaining_packets = multiple_packet_amount;
  }

  h.fields.payload_or_request_size = data;

  switch (direction) {
  case DIRECTION_STA_MASTER:
    h.fields.packet_direction = DIRECTION_STA_MASTER;
    break;
  case DIRECTION_MASTER_STA:
    h.fields.packet_direction = DIRECTION_MASTER_STA;
    break;
  case DIRECTION_MASTER_SLAVE:
    h.fields.packet_direction = DIRECTION_MASTER_SLAVE;
    break;
  case DIRECTION_SLAVE_MASTER:
    h.fields.packet_direction = DIRECTION_SLAVE_MASTER;
    break;
  default:
    // invalid packet
    break;
  }

  if (is_response) {
    switch (packet_type) {
    case RESPONSE_TEMPERATURE:
      h.fields.packet_type = 3;

      break;
    case RESPONSE_MICROPHONE:
      h.fields.packet_type = 1;
    }
  } else {
    switch (packet_type) {
    case REQUEST_TEMPERATURE:
      h.fields.packet_type = 2;

      break;
    case REQUEST_MICROPHONE:
      h.fields.packet_type = 0;
    }
  }
  return h;
}

packet_t packet_init(header_t header, uint8_t *data, size_t length) {
  packet_t p;
  p.header = header;

  for (size_t i = 0; i < 0; i++) {
    p.payload[i] = data[i];
  }
  return p;
}
