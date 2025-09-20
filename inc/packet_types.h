#pragma once

#include "packet.h"

/**
 * @brief Request packets enumeration.
 */
typedef enum request_packet {
  REQUEST_TEMPERATURE = 0,
  REQUEST_MICROPHONE = 1
} REQUEST_PACKET;

typedef enum response_packet {
  RESPONSE_TEMPERATURE = 0,
  RESPONSE_MICROPHONE = 1,
} RESPONSE_PACKET;

typedef enum direction_packet {
  DIRECTION_STA_MASTER = 0,
  DIRECTION_MASTER_STA = 1,
  DIRECTION_MASTER_SLAVE = 2,
  DIRECTION_SLAVE_MASTER = 3
} DIRECTION_PACKET;
