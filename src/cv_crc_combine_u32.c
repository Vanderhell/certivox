#include <certivox/cv_crc_combine_u32.h>

static uint32_t matrix_times(const uint32_t matrix[32], uint32_t vector)
{
    uint32_t sum = 0u;
    unsigned index = 0u;
    while (vector != 0u) {
        if ((vector & 1u) != 0u) {
            sum ^= matrix[index];
        }
        vector >>= 1u;
        ++index;
    }
    return sum;
}

static void matrix_square(uint32_t square[32], const uint32_t matrix[32])
{
    for (unsigned n = 0u; n < 32u; ++n) {
        square[n] = matrix_times(matrix, matrix[n]);
    }
}

uint32_t cv_crc_combine_u32(uint32_t crc_a, uint32_t crc_b, uint32_t length_b)
{
    if (length_b == 0u) {
        return crc_a;
    }
    uint32_t odd[32];
    uint32_t even[32];
    odd[0] = UINT32_C(0xEDB88320);
    uint32_t row = 1u;
    for (unsigned n = 1u; n < 32u; ++n) {
        odd[n] = row;
        row <<= 1u;
    }
    matrix_square(even, odd);
    matrix_square(odd, even);
    do {
        matrix_square(even, odd);
        if ((length_b & 1u) != 0u) {
            crc_a = matrix_times(even, crc_a);
        }
        length_b >>= 1u;
        if (length_b == 0u) {
            break;
        }
        matrix_square(odd, even);
        if ((length_b & 1u) != 0u) {
            crc_a = matrix_times(odd, crc_a);
        }
        length_b >>= 1u;
    } while (length_b != 0u);
    return crc_a ^ crc_b;
}
