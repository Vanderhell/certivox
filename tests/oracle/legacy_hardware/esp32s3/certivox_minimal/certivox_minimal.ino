#include <Arduino.h>

extern "C" {
#include "../certivox_hw/certivox/cv_crc32_compute.h"
#include "../certivox_hw/impl/cv_crc32_compute.c"
}

static volatile uint32_t sink;

void setup()
{
    static const uint8_t payload[] = "certivox-footprint";
    uint32_t value = 0u;
    (void)cv_crc32_compute(payload, sizeof(payload) - 1u, &value);
    sink = value;
    Serial.begin(115200);
    Serial.printf("CERTIVOX_MINIMAL|crc32=%u\n", (unsigned)sink);
}

void loop()
{
    delay(1000);
}
