#include <certivox/crc8_compute.h>
#include <certivox/crc16_compute.h>
#include <certivox/crc24_compute.h>
#include <certivox/crc32_compute.h>
#include <certivox/crc64_compute.h>
#include <certivox/crc_combine_u32.h>
#include <certivox/crc_reflect_poly_u32.h>
#include <certivox/crc_update_byte_u32.h>
#include <certivox/hamming74_decode.h>
#include <certivox/hamming74_encode.h>
#include <certivox/hamming84_decode.h>
#include <certivox/hamming84_encode.h>
#include <certivox/secded_check_u32.h>
#include <certivox/secded_parity_u32.h>

int main(void)
{
    return certivox_crc_combine_u32(0u, 0u, 0u) == 0u ? 0 : 1;
}
