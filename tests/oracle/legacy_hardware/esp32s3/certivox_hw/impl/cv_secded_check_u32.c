#include <certivox/cv_secded_check_u32.h>
#include <stddef.h>

cv_secded_status cv_secded_check_u32(uint32_t received_data, uint8_t received_check_bits,
                                     uint32_t *corrected_data)
{
    if (corrected_data == NULL || (received_check_bits & UINT8_C(0x80)) != 0u) {
        return CV_SECDED_INVALID;
    }
    uint8_t syndrome = 0u;
    uint8_t overall = 0u;
    unsigned data_bit = 0u;
    unsigned check_bit = 0u;
    for (unsigned position = 1u; position <= 38u; ++position) {
        uint8_t bit;
        if ((position & (position - 1u)) == 0u) {
            bit = (uint8_t)((received_check_bits >> check_bit) & 1u);
            ++check_bit;
        } else {
            bit = (uint8_t)((received_data >> data_bit) & 1u);
            ++data_bit;
        }
        if (bit != 0u) {
            syndrome ^= (uint8_t)position;
            overall ^= 1u;
        }
    }
    overall ^= (uint8_t)((received_check_bits >> 6u) & 1u);
    if (overall == 0u) {
        if (syndrome != 0u) {
            return CV_SECDED_UNCORRECTABLE;
        }
        *corrected_data = received_data;
        return CV_SECDED_CLEAN;
    }
    if (syndrome > 38u) {
        return CV_SECDED_UNCORRECTABLE;
    }
    if (syndrome != 0u) {
        data_bit = 0u;
        for (unsigned position = 1u; position <= 38u; ++position) {
            if ((position & (position - 1u)) == 0u) {
                continue;
            }
            if (position == syndrome) {
                received_data ^= UINT32_C(1) << data_bit;
                break;
            }
            ++data_bit;
        }
    }
    *corrected_data = received_data;
    return CV_SECDED_CORRECTED;
}
