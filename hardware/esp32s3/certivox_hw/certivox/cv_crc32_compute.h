#ifndef CV_CRC32_COMPUTE_H
#define CV_CRC32_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
bool cv_crc32_compute(const uint8_t *data, size_t length, uint32_t *crc);
#endif
