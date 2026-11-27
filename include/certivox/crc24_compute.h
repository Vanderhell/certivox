#ifndef CERTIVOX_CRC24_COMPUTE_H
#define CERTIVOX_CRC24_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool certivox_crc24_compute(const uint8_t *data, size_t length, uint32_t *crc);

#ifdef __cplusplus
}
#endif

#endif /* CERTIVOX_CRC24_COMPUTE_H */
