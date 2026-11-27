#ifndef CERTIVOX_CRC16_COMPUTE_H
#define CERTIVOX_CRC16_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
bool certivox_crc16_compute(const uint8_t *data, size_t length, uint16_t *crc);
#endif
