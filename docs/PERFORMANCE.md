# Performance

## Hardware evidence inherited from source records

The source ESP32-S3 records report 240 MHz measurements for the original
implementations. They are retained for traceability only. Reported firmware
sizes include Arduino/runtime, harness, test buffers and oracle data; they are
not Certivox-only `.text`, `.rodata`, `.data` or `.bss` measurements.

| Group | Hardware status | Measurement status |
|---|---|---|
| CRC profiles and helpers | ESP32-S3 PASS in source records | Cycles recorded; isolated footprint pending |
| Hamming(7,4)/(8,4) | ESP32-S3 PASS in source records | Cycles recorded; isolated footprint pending |
| SECDED(39,32) | ESP32-S3 PASS in source records | Cycles recorded; isolated footprint pending |

## Host benchmark

`bench/host_benchmark.c` is a smoke benchmark only. Host clock ticks are not
MCU cycle measurements and must not be compared with the ESP32-S3 records.

No isolated product footprint or post-extraction MCU measurement is claimed in
this release candidate.
