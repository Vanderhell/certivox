#ifndef CV_CRC64_COMPUTE_H
#define CV_CRC64_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
bool cv_crc64_compute(const uint8_t *data, size_t length, uint64_t *crc);
#endif
