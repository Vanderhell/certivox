# ESP32-S3 device result: `cv_crc16_compute`

## Purpose and category

The accepted production primitive computes CRC-16/MODBUS: reflected polynomial `0xA001` (normal `0x8005`), initial state `0xFFFF`, no final XOR. Category: Modbus RTU/ASCII serial communications and frame error detection.

## Board and build

- ESP32-S3 Xtensa LX7 at 240 MHz; COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Accepted production implementation compiled into a standalone device harness.
- Sketch footprint: 310,456 bytes flash and 22,960 bytes static RAM including runtime, generated 512-byte lookup table, 257-byte payload buffer, and harness; not primitive-only.

## Device checks

- Empty input returns `0xFFFF`; null-data/zero-length accepted; null data/nonzero length and null output rejected, with output sentinel unchanged on failure.
- Standard check string `123456789` returns `0x4B37`.
- Exhaustive 256 one-byte messages compared with a separate table-driven oracle.
- 200,000 deterministic random messages of lengths 0..257, seed `0x524D0122`, compared against the table oracle.
- Result: PASS; checksum `42420866`.
- Fixed 32-byte message timing: 25 batches of 10,000 calls; mean 2,004.43 cycles/call, range 2,004.34–2,004.50 cycles/call.
- Stress timing includes variable payload generation and table-oracle checks; batch timing measures repeated CRC calls.

## Raw serial result

```text
RMDEVICE|cv_crc16_compute|PASS|stress=200000|exhaustive=256|checksum=42420866|stress_cycles=19352880|batch=10000|min_cycles_x100=200434|mean_cycles_x100=200443|max_cycles_x100=200450|cpu_mhz=240
```

## Use and limitations

Useful for Modbus frame CRC generation and validation where this exact profile is specified. Other CRC-16 profiles, including CCITT and X-25, are not interchangeable. Hardware CRC or a lookup-table implementation may improve throughput at added peripheral or ROM cost. CRC is error detection, not authentication; returned numeric byte order is the caller's protocol responsibility.
