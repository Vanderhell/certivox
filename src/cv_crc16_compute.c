#include <certivox/cv_crc16_compute.h>

bool cv_crc16_compute(const uint8_t *data, size_t length, uint16_t *crc)
{
    if (crc == NULL || (data == NULL && length != 0u)) {
        return false;
    }
    uint16_t value = UINT16_C(0xFFFF);
    for (size_t i = 0u; i < length; ++i) {
        value ^= data[i];
        for (unsigned bit = 0u; bit < 8u; ++bit) {
            value = (value & UINT16_C(1)) != 0u
                ? (uint16_t)((value >> 1u) ^ UINT16_C(0xA001))
                : (uint16_t)(value >> 1u);
        }
    }
    *crc = value;
    return true;
}
