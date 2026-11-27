#include <Arduino.h>

static volatile uint32_t sink;

void setup()
{
    sink = 0xC3A7F00Du;
    Serial.begin(115200);
    Serial.printf("CERTIVOX_MINIMAL|crc32=%u\n", (unsigned)sink);
}

void loop()
{
    delay(1000);
}
