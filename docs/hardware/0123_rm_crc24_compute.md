# ESP32-S3 device result: `cv_crc24_compute`

## Purpose and category

This specialized primitive computes CRC-24/OpenPGP: polynomial `0x864CFB`, initial state `0xB704CE`, no reflection, xorout zero. It serves OpenPGP packet checksum compatibility, not generic packet integrity.

## Board and build

- ESP32-S3 Xtensa LX7 at 240 MHz; COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Isolated SPECIALIZED implementation; no production integration.
- Sketch footprint: 310,468 bytes flash and 23,472 bytes static RAM including runtime, 1 KiB lookup oracle, 257-byte input buffer, and harness; not primitive-only.

## Device checks

- Empty payload returns `0xB704CE`; null-data/zero-length valid; invalid null data and null output rejected with output preservation.
- Check string `123456789` returns `0x21CF02`.
- Exhaustive 256 one-byte payloads compared with a table-driven oracle.
- 200,000 deterministic variable-length payloads from 0 to 257 bytes (seed `0x524D0123`) compared with the oracle; each result also verified to fit 24 bits.
- Result: PASS; checksum `599957149`.
- 32-byte fixed payload: 25 batches of 10,000 calls, mean 2,455.59 cycles/call, range 2,455.56–2,455.67 cycles/call.
- Stress timing includes payload generation and table oracle; dedicated batches time repeated CRC calls.

## Raw serial result

```text
RMDEVICE|cv_crc24_compute|PASS|stress=200000|exhaustive=256|checksum=599957149|stress_cycles=880029755|batch=10000|min_cycles_x100=245556|mean_cycles_x100=245559|max_cycles_x100=245567|cpu_mhz=240
```

## Use and limitations

Useful only when matching CRC-24/OpenPGP packet checksums. LTE-A, BLE, and other CRC-24 profiles have different parameters. Hardware CRC or a lookup table may improve throughput; CRC is not authentication. Only the low 24 bits of the result are meaningful.
