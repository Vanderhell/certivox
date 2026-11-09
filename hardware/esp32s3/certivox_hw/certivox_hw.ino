#include <Arduino.h>
#include <esp_cpu.h>
extern "C" {
#include <certivox/cv_crc16_compute.h>
#include <certivox/cv_crc24_compute.h>
#include <certivox/cv_crc32_compute.h>
#include <certivox/cv_crc64_compute.h>
#include <certivox/cv_crc8_compute.h>
#include <certivox/cv_crc_combine_u32.h>
#include <certivox/cv_crc_reflect_poly_u32.h>
#include <certivox/cv_hamming74_decode.h>
#include <certivox/cv_hamming74_encode.h>
#include <certivox/cv_hamming84_decode.h>
#include <certivox/cv_hamming84_encode.h>
#include <certivox/cv_secded_check_u32.h>
#include <certivox/cv_secded_parity_u32.h>

#include "impl/cv_crc16_compute.c"
#include "impl/cv_crc24_compute.c"
#include "impl/cv_crc32_compute.c"
#include "impl/cv_crc64_compute.c"
#include "impl/cv_crc8_compute.c"
#include "impl/cv_crc_combine_u32.c"
#include "impl/cv_crc_reflect_poly_u32.c"
#include "impl/cv_hamming74_decode.c"
#include "impl/cv_hamming74_encode.c"
#include "impl/cv_hamming84_decode.c"
#include "impl/cv_hamming84_encode.c"
#include "impl/cv_secded_check_u32.c"
#include "impl/cv_secded_parity_u32.c"
}

static bool check(bool condition)
{
    return condition;
}

static uint32_t crc32_oracle(const uint8_t *data, size_t length)
{
    uint32_t value = UINT32_MAX;
    for (size_t i = 0u; i < length; ++i) {
        value ^= data[i];
        for (unsigned bit = 0u; bit < 8u; ++bit) {
            value = (value & 1u) != 0u
                ? (value >> 1u) ^ UINT32_C(0xEDB88320)
                : value >> 1u;
        }
    }
    return value ^ UINT32_MAX;
}

static uint32_t random_state = UINT32_C(0xC3A7F00D);

static uint32_t next_random(void)
{
    uint32_t value = random_state;
    value ^= value << 13u;
    value ^= value >> 17u;
    value ^= value << 5u;
    random_state = value;
    return value;
}

static bool run_checks(uint32_t *checksum, uint32_t *cycle_min, uint32_t *cycle_max)
{
    static const uint8_t vector[] = "123456789";
    bool pass = true;
    uint8_t crc8 = 0u;
    uint16_t crc16 = 0u;
    uint32_t crc24 = 0u;
    uint32_t crc32 = 0u;
    uint64_t crc64 = 0u;
    pass &= check(cv_crc8_compute(vector, 9u, &crc8) && crc8 == UINT8_C(0xF4));
    pass &= check(cv_crc16_compute(vector, 9u, &crc16) && crc16 == UINT16_C(0x4B37));
    pass &= check(cv_crc24_compute(vector, 9u, &crc24) && crc24 == UINT32_C(0x21CF02));
    pass &= check(cv_crc32_compute(vector, 9u, &crc32) && crc32 == UINT32_C(0xCBF43926));
    pass &= check(cv_crc64_compute(vector, 9u, &crc64) && crc64 == UINT64_C(0x6C40DF5F0B497347));

    uint32_t reflected = 0u;
    pass &= check(cv_crc_reflect_poly_u32(UINT32_C(0x04C11DB7), 32u, &reflected)
        && reflected == UINT32_C(0xEDB88320));

    uint32_t left = 0u;
    uint32_t right = 0u;
    uint32_t whole = 0u;
    pass &= check(cv_crc32_compute((const uint8_t *)"1234", 4u, &left));
    pass &= check(cv_crc32_compute((const uint8_t *)"56789", 5u, &right));
    pass &= check(cv_crc32_compute(vector, 9u, &whole));
    pass &= check(cv_crc_combine_u32(left, right, 5u) == whole);

    uint8_t encoded74 = 0u;
    uint8_t decoded74 = 0u;
    for (uint8_t nibble = 0u; nibble < 16u; ++nibble) {
        pass &= check(cv_hamming74_encode(nibble, &encoded74));
        pass &= check(cv_hamming74_decode(encoded74, &decoded74) == CV_HAMMING74_SYNDROME_ZERO
            && decoded74 == nibble);
        for (unsigned bit = 0u; bit < 7u; ++bit) {
            pass &= check(cv_hamming74_decode((uint8_t)(encoded74 ^ (1u << bit)), &decoded74)
                == CV_HAMMING74_SYNDROME_NONZERO && decoded74 == nibble);
        }
    }

    uint8_t encoded84 = 0u;
    uint8_t decoded84 = 0u;
    pass &= check(cv_hamming84_encode(UINT8_C(0x0B), &encoded84));
    pass &= check(cv_hamming84_decode((uint8_t)(encoded84 ^ UINT8_C(0x04)), &decoded84)
        == CV_HAMMING84_CORRECTED && decoded84 == UINT8_C(0x0B));
    pass &= check(cv_hamming84_decode((uint8_t)(encoded84 ^ UINT8_C(0x06)), &decoded84)
        == CV_HAMMING84_UNCORRECTABLE);

    const uint32_t protected_word = UINT32_C(0xA55A1234);
    uint8_t check_bits = 0u;
    uint32_t corrected_word = 0u;
    pass &= check(cv_secded_parity_u32(protected_word, &check_bits));
    pass &= check(cv_secded_check_u32(protected_word, check_bits, &corrected_word)
        == CV_SECDED_CLEAN && corrected_word == protected_word);

    static uint8_t payload[64];
    uint32_t local_checksum = 0u;
    for (unsigned trial = 0u; trial < 10000u; ++trial) {
        const size_t length = next_random() % sizeof(payload);
        for (size_t i = 0u; i < length; ++i) {
            payload[i] = (uint8_t)next_random();
        }
        pass &= check(cv_crc32_compute(payload, length, &crc32)
            && crc32 == crc32_oracle(payload, length));
        local_checksum = (local_checksum * 33u) ^ crc32 ^ (uint32_t)length;
    }

    uint32_t min_cycles = UINT32_MAX;
    uint32_t max_cycles = 0u;
    for (unsigned batch = 0u; batch < 10u; ++batch) {
        for (unsigned i = 0u; i < 32u; ++i) {
            payload[i] = (uint8_t)(i * 37u + 11u);
        }
        const uint32_t start = esp_cpu_get_cycle_count();
        for (unsigned call = 0u; call < 10000u; ++call) {
            pass &= check(cv_crc32_compute(payload, 32u, &crc32));
            local_checksum ^= crc32;
        }
        const uint32_t elapsed = esp_cpu_get_cycle_count() - start;
        if (elapsed < min_cycles) min_cycles = elapsed;
        if (elapsed > max_cycles) max_cycles = elapsed;
    }

    *checksum = local_checksum;
    *cycle_min = min_cycles / 10000u;
    *cycle_max = max_cycles / 10000u;
    return pass;
}

void setup()
{
    Serial.begin(115200);
    const uint32_t wait_start = millis();
    while (!Serial && (millis() - wait_start) < 5000u) {
        delay(10);
    }
    uint32_t checksum = 0u;
    uint32_t cycle_min = 0u;
    uint32_t cycle_max = 0u;
    const bool pass = run_checks(&checksum, &cycle_min, &cycle_max);
    Serial.printf("CERTIVOX_HW|%s|mcu=ESP32-S3|cpu_mhz=%u|tests=10000+exhaustive|checksum=%u|crc32_cycles_min=%u|crc32_cycles_max=%u\n",
        pass ? "PASS" : "FAIL", getCpuFrequencyMhz(), checksum, cycle_min, cycle_max);
}

void loop()
{
    delay(1000);
}
