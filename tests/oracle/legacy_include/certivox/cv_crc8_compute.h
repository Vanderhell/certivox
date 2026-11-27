#ifndef CV_CRC8_COMPUTE_H
#define CV_CRC8_COMPUTE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
bool cv_crc8_compute(const uint8_t *data, size_t length, uint8_t *crc);
#endif
