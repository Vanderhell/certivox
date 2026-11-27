#ifndef CERTIVOX_CRC8_COMPUTE_H
#define CERTIVOX_CRC8_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool certivox_crc8_compute(const uint8_t *data, size_t length, uint8_t *crc);

#ifdef __cplusplus
}
#endif

#endif /* CERTIVOX_CRC8_COMPUTE_H */
