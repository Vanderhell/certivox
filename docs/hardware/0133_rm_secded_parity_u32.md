# ESP32-S3 result: `cv_secded_parity_u32`

## Purpose and category

Produces the seven check bits for extended Hamming(39,32) on a 32-bit data word. Category: software ECC for SRAM words, compact records, or retained embedded state where hardware ECC is unavailable.

## Device and build

- ESP32-S3 Xtensa LX7 at 240 MHz, COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Boot marker: `RMBOOT|candidate=133|build=20261029a`.
- Sketch footprint: 310,840 bytes flash and 22,192 bytes static RAM, including runtime, oracle, 256 codewords, and harness; not primitive-only. The production routine uses bounded scalar state and no lookup table.

## Tests and results

- PASS: exhaustive 8-bit data subdomain, all 256 values, matched an independent six-mask parity oracle.
- PASS: 1,536 Hamming parity checks and even total parity on the generated 39-bit codewords.
- PASS: all 65,280 ordered distinct pairs within the 256-word subdomain had distance at least 4; minimum observed distance 4.
- PASS: all 32 one-hot input basis vectors matched the oracle and yielded codeword weight at least 4.
- PASS: 100,000 seeded full-width `uint32_t` values; seed `0x524D0133`, checksum `529982445`.
- 10 batches of 100,000 calls: mean 370.77 cycles/call, range 370.76..370.78 cycles/call at 240 MHz.

## Raw serial result

```text
RMDEVICE|cv_secded_parity_u32|PASS|exhaustive=256|syndrome_checks=1536|pairs=65280|min_distance=4|basis=32|stress=100000|checksum=529982445|batch=100000|mean_cycles_x100=37077|min_cycles_x100=37076|max_cycles_x100=37078|cpu_mhz=240
```

## Use and limits

Useful when a device needs software-generated ECC checks for fixed 32-bit words and has no hardware ECC. This primitive only generates check bits; a matching decoder/corrector is required. Confirm the target's bit layout and parity convention, and use interleaving or stronger coding for burst errors. SECDED does not protect against malicious modification.
