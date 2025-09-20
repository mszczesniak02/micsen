#include "packet_types.h"

extern uint8_t PACKET_DATA_BUFFER[PACKET_MAX_PAYLOAD_SIZE];

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
