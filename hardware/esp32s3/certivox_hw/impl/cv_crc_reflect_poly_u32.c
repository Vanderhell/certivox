#include <certivox/cv_crc_reflect_poly_u32.h>
#include <stddef.h>

bool cv_crc_reflect_poly_u32(uint32_t polynomial, uint8_t width, uint32_t *reflected)
{
    if (reflected == NULL || width == 0u || width > 32u
        || (width < 32u && (polynomial >> width) != 0u)) {
        return false;
    }
    uint32_t result = 0u;
    for (unsigned bit = 0u; bit < width; ++bit) {
        result = (result << 1u) | (polynomial & 1u);
        polynomial >>= 1u;
    }
    *reflected = result;
    return true;
}
