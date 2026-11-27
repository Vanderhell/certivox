#include <certivox/cv_crc32_compute.h>

bool cv_crc32_compute(const uint8_t *data, size_t length, uint32_t *crc)
{
    if (crc == NULL || (data == NULL && length != 0u)) {
        return false;
    }
    uint32_t value = UINT32_MAX;
    for (size_t i = 0u; i < length; ++i) {
        value ^= data[i];
        for (unsigned bit = 0u; bit < 8u; ++bit) {
            value = (value & UINT32_C(1)) != 0u
                ? (value >> 1u) ^ UINT32_C(0xEDB88320)
                : value >> 1u;
        }
    }
    *crc = value ^ UINT32_MAX;
    return true;
}
