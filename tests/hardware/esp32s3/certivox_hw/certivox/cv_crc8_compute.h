#ifndef CERTIVOX_CRC8_COMPUTE_H
#define CERTIVOX_CRC8_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
bool certivox_crc8_compute(const uint8_t *data, size_t length, uint8_t *crc);
#endif
