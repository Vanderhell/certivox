# ESP32-S3 result: `cv_crc_reflect_poly_u32`

## Purpose and category

Reverses the low `width` bits of a normal-form CRC polynomial into the reflected form used by right-shifting CRC update routines. Category: CRC configuration and protocol setup.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=139|build=20261029a`.
- Sketch footprint: 310,828 bytes flash and 22,192 bytes static RAM, including runtime, oracle, and harness; not primitive-only.
- Target compilation found `NULL` was used without explicitly including `<stddef.h>` in the production source. The standard header is now included; behavior is unchanged.

## Tests and results

- PASS: exhaustive 510 valid polynomial/width combinations for widths 1..8 against the independent bit-index oracle; reflection involution held for all.
- PASS: known CRC-8, CRC-16/MODBUS, and CRC-32/ISO-HDLC reflection pairs.
- PASS: invalid widths, polynomial high bits, null output, and unchanged output sentinel.
- PASS: 100,000 deterministic valid full-width inputs plus 10,000 mixed valid/invalid domain cases; seed `0x524D0139`, checksum `3297255382`.
- 10 batches of 100,000 calls: width 1 mean 58.29 cycles/call; width 32 mean 306.56 cycles/call at 240 MHz.

## Raw serial result

```text
RMDEVICE|cv_crc_reflect_poly_u32|PASS|exhaustive=510|stress=100000|domain_stress=10000|checksum=3297255382|batch=100000|width1_mean_x100=5829|width1_min=5829|width1_max=5829|width32_mean_x100=30656|width32_min=30656|width32_max=30656|cpu_mhz=240
```

## Use and limits

Useful when CRC parameters arrive as normal-form polynomials at configuration time. Do not run this in a per-byte CRC path; precompute constant profiles. Width is limited to 1..32 and only the explicit polynomial bits are reflected, not CRC initialization, augmentation, or final XOR settings.
