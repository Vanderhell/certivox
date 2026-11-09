#include <certivox/cv_crc8_compute.h>

bool cv_crc8_compute(const uint8_t *data, size_t length, uint8_t *crc)
{
    if (crc == NULL || (data == NULL && length != 0u)) {
        return false;
    }
    uint8_t value = 0u;
    for (size_t i = 0u; i < length; ++i) {
        value ^= data[i];
        for (unsigned bit = 0u; bit < 8u; ++bit) {
            value = (value & UINT8_C(0x80)) != 0u
                ? (uint8_t)((uint8_t)(value << 1u) ^ UINT8_C(0x07))
                : (uint8_t)(value << 1u);
        }
    }
    *crc = value;
    return true;
}
