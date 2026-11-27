#ifndef CERTIVOX_CRC64_COMPUTE_H
#define CERTIVOX_CRC64_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool certivox_crc64_compute(const uint8_t *data, size_t length, uint64_t *crc);

#ifdef __cplusplus
}
#endif

#endif /* CERTIVOX_CRC64_COMPUTE_H */
