#include <certivox/cv_crc8_compute.h>
#include <certivox/cv_crc16_compute.h>
#include <certivox/cv_crc24_compute.h>
#include <certivox/cv_crc32_compute.h>
#include <certivox/cv_crc64_compute.h>
#include <certivox/cv_crc_update_byte_u32.h>
#include <certivox/cv_crc_combine_u32.h>
#include <certivox/cv_crc_reflect_poly_u32.h>
#include <certivox/cv_hamming74_encode.h>
#include <certivox/cv_hamming74_decode.h>
#include <certivox/cv_hamming84_encode.h>
#include <certivox/cv_hamming84_decode.h>
#include <certivox/cv_secded_parity_u32.h>
#include <certivox/cv_secded_check_u32.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define CHECK(condition) do { \
    if (!(condition)) { \
        (void)fprintf(stderr, "audit check failed at line %d\n", __LINE__); \
        return 1; \
    } \
} while (0)

#define CASES UINT32_C(100000)

static uint32_t random_state = UINT32_C(0xC37A91E5);

static uint32_t random_u32(void)
{
    uint32_t value = random_state;
    value ^= value << 13u;
    value ^= value >> 17u;
    value ^= value << 5u;
    random_state = value;
    return value;
}

static uint8_t table8[256];
static uint16_t table16[256];
static uint32_t table24[256];
static uint32_t table32[256];
static uint64_t table64[256];

static void make_crc_tables(void)
{
    unsigned i;
    for (i = 0u; i < 256u; ++i) {
        uint8_t c8 = (uint8_t)i;
        uint16_t c16 = (uint16_t)i;
        uint32_t c24 = (uint32_t)i << 16u;
        uint32_t c32 = (uint32_t)i;
        uint64_t c64 = (uint64_t)i << 56u;
        unsigned bit;
        for (bit = 0u; bit < 8u; ++bit) {
            c8 = (uint8_t)((c8 & UINT8_C(0x80)) != 0u
                ? (uint8_t)(c8 << 1u) ^ UINT8_C(0x07) : (uint8_t)(c8 << 1u));
            c16 = (uint16_t)((c16 & 1u) != 0u
                ? (c16 >> 1u) ^ UINT16_C(0xA001) : c16 >> 1u);
            c24 = (c24 & UINT32_C(0x800000)) != 0u
                ? ((c24 << 1u) ^ UINT32_C(0x864CFB)) & UINT32_C(0xFFFFFF)
                : (c24 << 1u) & UINT32_C(0xFFFFFF);
            c32 = (c32 & 1u) != 0u
                ? (c32 >> 1u) ^ UINT32_C(0xEDB88320) : c32 >> 1u;
            c64 = (c64 & (UINT64_C(1) << 63u)) != 0u
                ? (c64 << 1u) ^ UINT64_C(0x42F0E1EBA9EA3693) : c64 << 1u;
        }
        table8[i] = c8;
        table16[i] = c16;
        table24[i] = c24;
        table32[i] = c32;
        table64[i] = c64;
    }
}

static uint8_t ref_crc8(const uint8_t *data, size_t length)
{
    uint8_t value = 0u;
    size_t i;
    for (i = 0u; i < length; ++i) value = table8[(uint8_t)(value ^ data[i])];
    return value;
}

static uint16_t ref_crc16(const uint8_t *data, size_t length)
{
    uint16_t value = UINT16_C(0xFFFF);
    size_t i;
    for (i = 0u; i < length; ++i) {
        value = (uint16_t)((value >> 8u) ^ table16[(uint8_t)(value ^ data[i])]);
    }
    return value;
}

static uint32_t ref_crc24(const uint8_t *data, size_t length)
{
    uint32_t value = UINT32_C(0xB704CE);
    size_t i;
    for (i = 0u; i < length; ++i) {
        const uint8_t index = (uint8_t)((value >> 16u) ^ data[i]);
        value = ((value << 8u) & UINT32_C(0xFFFFFF)) ^ table24[index];
    }
    return value;
}

static uint32_t ref_crc32_raw_update(uint32_t value, const uint8_t *data, size_t length)
{
    size_t i;
    for (i = 0u; i < length; ++i) {
        value = (value >> 8u) ^ table32[(uint8_t)(value ^ data[i])];
    }
    return value;
}

static uint32_t ref_crc32(const uint8_t *data, size_t length)
{
    return ref_crc32_raw_update(UINT32_MAX, data, length) ^ UINT32_MAX;
}

static uint32_t ref_crc32_concat(const uint8_t *a, size_t alen, const uint8_t *b, size_t blen)
{
    uint32_t value = ref_crc32_raw_update(UINT32_MAX, a, alen);
    return ref_crc32_raw_update(value, b, blen) ^ UINT32_MAX;
}

static uint64_t ref_crc64(const uint8_t *data, size_t length)
{
    uint64_t value = 0u;
    size_t i;
    for (i = 0u; i < length; ++i) {
        const uint8_t index = (uint8_t)((value >> 56u) ^ data[i]);
        value = (value << 8u) ^ table64[index];
    }
    return value;
}

static uint32_t ref_crc_update(uint32_t state, uint8_t byte, const uint32_t table[256])
{
    return (state >> 8u) ^ table[(uint8_t)(state ^ byte)];
}

static uint32_t ref_reflect(uint32_t polynomial, unsigned width)
{
    uint32_t result = 0u;
    unsigned bit;
    for (bit = 0u; bit < width; ++bit) {
        if (((polynomial >> bit) & 1u) != 0u) result |= UINT32_C(1) << (width - bit - 1u);
    }
    return result;
}

static uint32_t ref_h74_encode(uint8_t nibble)
{
    uint32_t word = ((uint32_t)(nibble & 1u) << 2u)
        | ((uint32_t)((nibble >> 1u) & 1u) << 4u)
        | ((uint32_t)((nibble >> 2u) & 1u) << 5u)
        | ((uint32_t)((nibble >> 3u) & 1u) << 6u);
    unsigned parity_position;
    for (parity_position = 1u; parity_position <= 4u; parity_position <<= 1u) {
        unsigned parity = 0u;
        unsigned position;
        for (position = 1u; position <= 7u; ++position) {
            if (position != parity_position && (position & parity_position) != 0u) {
                parity ^= (unsigned)((word >> (position - 1u)) & 1u);
            }
        }
        word |= (uint32_t)parity << (parity_position - 1u);
    }
    return word;
}

static unsigned popcount_u32(uint32_t value)
{
    unsigned count = 0u;
    while (value != 0u) {
        value &= value - 1u;
        ++count;
    }
    return count;
}

static unsigned popcount_u8(uint8_t value)
{
    return popcount_u32((uint32_t)value);
}

static uint8_t ref_h84_encode(uint8_t nibble)
{
    const uint8_t low = (uint8_t)ref_h74_encode(nibble);
    return (uint8_t)(low | (uint8_t)((popcount_u8(low) & 1u) << 7u));
}

static cv_hamming74_status ref_h74_decode(uint8_t received, uint8_t *nibble)
{
    unsigned best_distance = 8u;
    uint8_t best = 0u;
    unsigned candidate;
    for (candidate = 0u; candidate < 16u; ++candidate) {
        const unsigned distance = popcount_u8((uint8_t)(received ^ ref_h74_encode((uint8_t)candidate)));
        if (distance < best_distance) {
            best_distance = distance;
            best = (uint8_t)candidate;
        }
    }
    *nibble = best;
    return best_distance == 0u ? CV_HAMMING74_SYNDROME_ZERO : CV_HAMMING74_SYNDROME_NONZERO;
}

static cv_hamming84_status ref_h84_decode(uint8_t received, uint8_t *nibble)
{
    unsigned best_distance = 9u;
    uint8_t best = 0u;
    unsigned candidate;
    for (candidate = 0u; candidate < 16u; ++candidate) {
        const unsigned distance = popcount_u8((uint8_t)(received ^ ref_h84_encode((uint8_t)candidate)));
        if (distance < best_distance) {
            best_distance = distance;
            best = (uint8_t)candidate;
        }
    }
    if (best_distance == 0u) {
        *nibble = best;
        return CV_HAMMING84_CLEAN;
    }
    if (best_distance == 1u) {
        *nibble = best;
        return CV_HAMMING84_CORRECTED;
    }
    return CV_HAMMING84_UNCORRECTABLE;
}

static uint8_t ref_secded_parity(uint32_t data)
{
    uint8_t parity = 0u;
    unsigned data_bit = 0u;
    unsigned position;
    unsigned bit;
    for (position = 1u; position <= 38u; ++position) {
        if ((position & (position - 1u)) == 0u) continue;
        if (((data >> data_bit) & 1u) != 0u) {
            for (bit = 0u; bit < 6u; ++bit) {
                if ((position & (1u << bit)) != 0u) parity ^= (uint8_t)(1u << bit);
            }
        }
        ++data_bit;
    }
    parity |= (uint8_t)((popcount_u32(data) + popcount_u8((uint8_t)(parity & 0x3Fu))) & 1u) << 6u;
    return parity;
}

static cv_secded_status ref_secded_check(uint32_t data, uint8_t checks, uint32_t *corrected)
{
    const uint8_t expected = ref_secded_parity(data);
    const uint8_t syndrome = (uint8_t)((checks ^ expected) & UINT8_C(0x3F));
    const unsigned overall = (popcount_u32(data) + popcount_u8((uint8_t)(checks & 0x7Fu))) & 1u;
    unsigned data_bit = 0u;
    unsigned position;
    if ((checks & UINT8_C(0x80)) != 0u) return CV_SECDED_INVALID;
    if (overall == 0u) {
        if (syndrome != 0u) return CV_SECDED_UNCORRECTABLE;
        *corrected = data;
        return CV_SECDED_CLEAN;
    }
    if (syndrome > 38u) return CV_SECDED_UNCORRECTABLE;
    *corrected = data;
    if (syndrome != 0u) {
        for (position = 1u; position <= 38u; ++position) {
            if ((position & (position - 1u)) == 0u) continue;
            if (position == syndrome) {
                *corrected ^= UINT32_C(1) << data_bit;
                break;
            }
            ++data_bit;
        }
    }
    return CV_SECDED_CORRECTED;
}

static void make_reflected_table(uint32_t polynomial, uint32_t table[256])
{
    unsigned i;
    for (i = 0u; i < 256u; ++i) {
        uint32_t value = i;
        unsigned bit;
        for (bit = 0u; bit < 8u; ++bit) {
            value = (value & 1u) != 0u ? (value >> 1u) ^ polynomial : value >> 1u;
        }
        table[i] = value;
    }
}

static int test_crc_differential(void)
{
    uint8_t payload[128];
    unsigned trial;
    make_crc_tables();
    for (trial = 0u; trial < CASES; ++trial) {
        const size_t length = (size_t)(random_u32() % (uint32_t)(sizeof(payload) + 1u));
        size_t i;
        uint8_t c8 = 0u;
        uint16_t c16 = 0u;
        uint32_t c24 = 0u;
        uint32_t c32 = 0u;
        uint64_t c64 = 0u;
        uint32_t stream = UINT32_MAX;
        for (i = 0u; i < sizeof(payload); ++i) payload[i] = (uint8_t)random_u32();
        CHECK(cv_crc8_compute(payload, length, &c8) && c8 == ref_crc8(payload, length));
        CHECK(cv_crc16_compute(payload, length, &c16) && c16 == ref_crc16(payload, length));
        CHECK(cv_crc24_compute(payload, length, &c24) && c24 == ref_crc24(payload, length));
        CHECK(cv_crc32_compute(payload, length, &c32) && c32 == ref_crc32(payload, length));
        CHECK(cv_crc64_compute(payload, length, &c64) && c64 == ref_crc64(payload, length));
        for (i = 0u; i < length; ++i) {
            stream = ref_crc_update(stream, payload[i], table32);
        }
        CHECK((stream ^ UINT32_MAX) == c32);
    }
    return 0;
}

static int test_crc_helpers(void)
{
    uint32_t trial;
    for (trial = 0u; trial < CASES; ++trial) {
        uint32_t polynomial = random_u32();
        const unsigned width = (unsigned)(random_u32() % 32u) + 1u;
        uint32_t mask = UINT32_MAX;
        uint32_t reflected = UINT32_C(0xA5A55A5A);
        if (width < 32u) mask = (UINT32_C(1) << width) - 1u;
        polynomial &= mask;
        CHECK(cv_crc_reflect_poly_u32(polynomial, (uint8_t)width, &reflected));
        CHECK(reflected == ref_reflect(polynomial, width));
    }
    {
        uint32_t out = UINT32_C(0xA5A55A5A);
        CHECK(!cv_crc_reflect_poly_u32(1u, 0u, &out) && out == UINT32_C(0xA5A55A5A));
        CHECK(!cv_crc_reflect_poly_u32(0u, 33u, &out) && out == UINT32_C(0xA5A55A5A));
        CHECK(!cv_crc_reflect_poly_u32(UINT32_C(0x100), 8u, &out) && out == UINT32_C(0xA5A55A5A));
        CHECK(!cv_crc_reflect_poly_u32(0u, 8u, NULL));
    }
    for (trial = 0u; trial < 1000u; ++trial) {
        uint32_t table[256];
        const uint32_t polynomial = (trial == 0u) ? 0u : (trial == 1u) ? UINT32_MAX : random_u32();
        unsigned j;
        make_reflected_table(polynomial, table);
        for (j = 0u; j < 100u; ++j) {
            const uint32_t state = random_u32();
            const uint8_t byte = (uint8_t)random_u32();
            CHECK(cv_crc_update_byte_u32(state, byte, polynomial)
                == ref_crc_update(state, byte, table));
        }
    }
    return 0;
}

static int test_crc_combine(void)
{
    uint8_t a[128];
    uint8_t b[128];
    uint8_t joined[256];
    unsigned trial;
    make_crc_tables();
    for (trial = 0u; trial < CASES; ++trial) {
        const size_t alen = (size_t)(random_u32() % 129u);
        const size_t blen = (size_t)(random_u32() % 129u);
        size_t i;
        for (i = 0u; i < alen; ++i) a[i] = (uint8_t)random_u32();
        for (i = 0u; i < blen; ++i) b[i] = (uint8_t)random_u32();
        for (i = 0u; i < alen; ++i) joined[i] = a[i];
        for (i = 0u; i < blen; ++i) joined[alen + i] = b[i];
        CHECK(cv_crc_combine_u32(ref_crc32(a, alen), ref_crc32(b, blen), (uint32_t)blen)
            == ref_crc32(joined, alen + blen));
    }
    {
        static uint8_t long_b[65536];
        static const uint8_t prefix[] = {UINT8_C(0x01), UINT8_C(0x80), UINT8_C(0xFE)};
        size_t i;
        uint32_t crc_a;
        uint32_t crc_b;
        for (i = 0u; i < sizeof(long_b); ++i) long_b[i] = (uint8_t)(i * 29u + 17u);
        crc_a = ref_crc32(prefix, sizeof(prefix));
        crc_b = ref_crc32(long_b, sizeof(long_b));
        CHECK(cv_crc_combine_u32(crc_a, crc_b, (uint32_t)sizeof(long_b))
            == ref_crc32_concat(prefix, sizeof(prefix), long_b, sizeof(long_b)));
        CHECK(cv_crc_combine_u32(0u, crc_b, (uint32_t)sizeof(long_b)) == crc_b);
    }
    CHECK(cv_crc_combine_u32(0u, 0u, UINT32_MAX) == 0u);
    /* Independent zlib 1.3.2 crc32_combine64 boundary vector. */
    CHECK(cv_crc_combine_u32(UINT32_C(0x12345678), UINT32_C(0x9ABCDEF0), UINT32_MAX)
        == UINT32_C(0x88888888));
    return 0;
}

static int test_hamming_exhaustive(void)
{
    unsigned value;
    for (value = 0u; value < 16u; ++value) {
        uint8_t actual = UINT8_C(0xA5);
        CHECK(cv_hamming74_encode((uint8_t)value, &actual));
        CHECK(actual == (uint8_t)ref_h74_encode((uint8_t)value));
        CHECK(cv_hamming84_encode((uint8_t)value, &actual));
        CHECK(actual == ref_h84_encode((uint8_t)value));
    }
    for (value = 0u; value < 128u; ++value) {
        uint8_t expected = UINT8_C(0xA5);
        uint8_t actual = UINT8_C(0xA5);
        const cv_hamming74_status ref_status = ref_h74_decode((uint8_t)value, &expected);
        CHECK(cv_hamming74_decode((uint8_t)value, &actual) == ref_status && actual == expected);
    }
    for (value = 0u; value < 16u; ++value) {
        const uint8_t code = (uint8_t)ref_h74_encode((uint8_t)value);
        unsigned a;
        for (a = 0u; a < 7u; ++a) {
            unsigned b;
            for (b = a + 1u; b < 7u; ++b) {
                uint8_t expected = UINT8_C(0xA5);
                uint8_t actual = UINT8_C(0xA5);
                const uint8_t damaged = (uint8_t)(code ^ (uint8_t)(1u << a) ^ (uint8_t)(1u << b));
                const cv_hamming74_status ref_status = ref_h74_decode(damaged, &expected);
                const cv_hamming74_status status = cv_hamming74_decode(damaged, &actual);
                CHECK(ref_status == CV_HAMMING74_SYNDROME_NONZERO);
                CHECK(status == ref_status && actual == expected && actual != (uint8_t)value);
            }
        }
    }
    for (value = 0u; value < 256u; ++value) {
        uint8_t expected = UINT8_C(0xA5);
        uint8_t actual = UINT8_C(0xA5);
        const cv_hamming84_status ref_status = ref_h84_decode((uint8_t)value, &expected);
        const cv_hamming84_status status = cv_hamming84_decode((uint8_t)value, &actual);
        CHECK(status == ref_status);
        if (status == CV_HAMMING84_UNCORRECTABLE) CHECK(actual == UINT8_C(0xA5));
        else CHECK(actual == expected);
    }
    {
        uint8_t output = UINT8_C(0xA5);
        CHECK(!cv_hamming74_encode(16u, &output) && output == UINT8_C(0xA5));
        CHECK(cv_hamming74_decode(UINT8_C(0x80), &output) == CV_HAMMING74_INVALID
            && output == UINT8_C(0xA5));
        CHECK(!cv_hamming84_encode(16u, &output) && output == UINT8_C(0xA5));
        CHECK(cv_hamming74_decode(0u, NULL) == CV_HAMMING74_INVALID);
        CHECK(cv_hamming84_decode(0u, NULL) == CV_HAMMING84_INVALID);
        CHECK(!cv_hamming74_encode(0u, NULL));
        CHECK(!cv_hamming84_encode(0u, NULL));
    }
    return 0;
}

static void flip_secded_position(uint32_t *data, uint8_t *checks, unsigned position)
{
    unsigned data_bit = 0u;
    unsigned check_bit = 0u;
    unsigned p;
    for (p = 1u; p <= 38u; ++p) {
        if ((p & (p - 1u)) == 0u) {
            if (p == position) *checks ^= (uint8_t)(1u << check_bit);
            ++check_bit;
        } else {
            if (p == position) *data ^= UINT32_C(1) << data_bit;
            ++data_bit;
        }
    }
    if (position == 39u) *checks ^= UINT8_C(0x40);
}

static int test_secded(void)
{
    static const uint32_t seeds[] = {
        0u, UINT32_MAX, UINT32_C(0x80000000), UINT32_C(0x7FFFFFFF), UINT32_C(0x12345678)
    };
    size_t seed_index;
    uint32_t trial;
    for (seed_index = 0u; seed_index < sizeof(seeds) / sizeof(seeds[0]); ++seed_index) {
        const uint32_t original = seeds[seed_index];
        uint8_t checks = UINT8_C(0xA5);
        uint32_t output = UINT32_C(0xA5A5A5A5);
        unsigned a;
        CHECK(cv_secded_parity_u32(original, &checks));
        CHECK(checks == ref_secded_parity(original));
        CHECK(cv_secded_check_u32(original, checks, &output) == CV_SECDED_CLEAN && output == original);
        for (a = 1u; a <= 39u; ++a) {
            uint32_t damaged = original;
            uint8_t damaged_checks = checks;
            output = UINT32_C(0xA5A5A5A5);
            flip_secded_position(&damaged, &damaged_checks, a);
            CHECK(cv_secded_check_u32(damaged, damaged_checks, &output) == CV_SECDED_CORRECTED
                && output == original);
            for (unsigned b = a + 1u; b <= 39u; ++b) {
                uint32_t double_data = original;
                uint8_t double_checks = checks;
                output = UINT32_C(0xA5A5A5A5);
                flip_secded_position(&double_data, &double_checks, a);
                flip_secded_position(&double_data, &double_checks, b);
                CHECK(cv_secded_check_u32(double_data, double_checks, &output)
                    == CV_SECDED_UNCORRECTABLE && output == UINT32_C(0xA5A5A5A5));
            }
        }
    }
    for (trial = 0u; trial < CASES; ++trial) {
        const uint32_t original = random_u32();
        const uint8_t checks = ref_secded_parity(original);
        uint8_t actual_checks = UINT8_C(0xFF);
        uint32_t output = UINT32_C(0xA5A5A5A5);
        uint32_t damaged;
        uint8_t damaged_checks;
        unsigned bit_a = (unsigned)(random_u32() % 39u) + 1u;
        unsigned bit_b = (unsigned)(random_u32() % 38u) + 1u;
        if (bit_b >= bit_a) ++bit_b;
        CHECK(cv_secded_parity_u32(original, &actual_checks) && actual_checks == checks);
        CHECK(cv_secded_check_u32(original, checks, &output) == CV_SECDED_CLEAN && output == original);

        damaged = original;
        damaged_checks = checks;
        flip_secded_position(&damaged, &damaged_checks, bit_a);
        output = UINT32_C(0xA5A5A5A5);
        {
            uint32_t reference_output = UINT32_C(0xA5A5A5A5);
            const cv_secded_status actual_status = cv_secded_check_u32(damaged, damaged_checks, &output);
            const cv_secded_status reference_status = ref_secded_check(damaged, damaged_checks, &reference_output);
            CHECK(actual_status == reference_status && output == reference_output);
        }
        CHECK(output == original);

        damaged = original;
        damaged_checks = checks;
        flip_secded_position(&damaged, &damaged_checks, bit_a);
        flip_secded_position(&damaged, &damaged_checks, bit_b);
        output = UINT32_C(0xA5A5A5A5);
        CHECK(cv_secded_check_u32(damaged, damaged_checks, &output)
            == CV_SECDED_UNCORRECTABLE && output == UINT32_C(0xA5A5A5A5));
    }
    {
        uint8_t checks = 0u;
        uint32_t output = UINT32_C(0xA5A5A5A5);
        CHECK(!cv_secded_parity_u32(0u, NULL));
        CHECK(cv_secded_check_u32(0u, UINT8_C(0x80), &output) == CV_SECDED_INVALID
            && output == UINT32_C(0xA5A5A5A5));
        CHECK(cv_secded_check_u32(0u, checks, NULL) == CV_SECDED_INVALID);
    }
    return 0;
}

int main(void)
{
    CHECK(test_crc_differential() == 0);
    CHECK(test_crc_helpers() == 0);
    CHECK(test_crc_combine() == 0);
    CHECK(test_hamming_exhaustive() == 0);
    CHECK(test_secded() == 0);
    (void)puts("14 API audit differential tests passed");
    return 0;
}
