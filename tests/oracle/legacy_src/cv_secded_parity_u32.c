#include <certivox/cv_secded_parity_u32.h>
#include <stddef.h>

bool cv_secded_parity_u32(uint32_t data, uint8_t *check_bits)
{
    if (check_bits == NULL) {
        return false;
    }
    uint8_t hamming = 0u;
    uint8_t overall = 0u;
    unsigned data_bit = 0u;
    for (unsigned position = 1u; position <= 38u; ++position) {
        if ((position & (position - 1u)) == 0u) {
            continue;
        }
        if (((data >> data_bit) & 1u) != 0u) {
            hamming ^= (uint8_t)position;
            overall ^= 1u;
        }
        ++data_bit;
    }
    for (unsigned bit = 0u; bit < 6u; ++bit) {
        overall ^= (uint8_t)((hamming >> bit) & 1u);
    }
    *check_bits = (uint8_t)(hamming | (overall << 6u));
    return true;
}
