#ifndef CERTIVOX_CRC32_COMPUTE_H
#define CERTIVOX_CRC32_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
bool certivox_crc32_compute(const uint8_t *data, size_t length, uint32_t *crc);
#endif
