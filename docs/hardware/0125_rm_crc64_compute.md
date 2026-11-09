# ESP32-S3 device result: `cv_crc64_compute`

## Purpose and category

Specialized CRC-64/ECMA-182 checksum for compatible data interchange or storage formats: width 64, polynomial `0x42F0E1EBA9EA3693`, init zero, no reflection, xorout zero. Empty payload produces zero.

## Board and build

- ESP32-S3 Xtensa LX7 at 240 MHz; COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Isolated SPECIALIZED implementation; no production integration.
- Sketch footprint: 310,544 bytes flash and 24,504 bytes static RAM including runtime, 2 KiB lookup oracle, 257-byte payload buffer, and harness; not primitive-only. The 64-bit arithmetic cost is target/compiler dependent.

## Device checks

- Empty input returns zero; null-data/zero-length accepted; invalid null data/nonzero length and null output rejected with output preservation.
- Standard `123456789` check vector returns `0x6C40DF5F0B497347`.
- Exhaustive 256 one-byte payloads compared to an independently structured table-driven oracle.
- 50,000 deterministic variable-length payloads (0..257 bytes), seed `0x524D0125`, differential-checked against the table oracle.
- Result: PASS; checksum `4107712918`.
- Fixed 32-byte payload: 25 batches of 10,000 calls, mean 2,587.19 cycles/call, range 2,587.10–2,587.24 cycles/call.
- Stress timing includes payload generation and oracle checks; dedicated timing measures repeated CRC calls.

## Raw serial result

```text
RMDEVICE|cv_crc64_compute|PASS|stress=50000|exhaustive=256|checksum=4107712918|stress_cycles=1332772813|batch=10000|min_cycles_x100=258710|mean_cycles_x100=258719|max_cycles_x100=258724|cpu_mhz=240
```

## Use and limitations

Useful only when a format explicitly requires this CRC-64/ECMA-182 profile and compatibility justifies 64-bit per-bit processing. CRC-64/XZ and CRC-64/WE are different profiles. On small MCUs, lower-width CRCs or a hardware/table implementation may be cheaper; CRC is not cryptographic authentication.
