# ESP32-S3 device result: `cv_crc32_compute`

## Purpose and category

The accepted production function computes CRC-32/ISO-HDLC (reflected polynomial `0xEDB88320`, initial/final XOR `UINT32_MAX`), used by Ethernet and ZIP-like formats. Category: packet/storage error detection.

## Board and build

- ESP32-S3 Xtensa LX7 at 240 MHz; COM36.
- Arduino-ESP32 core 3.3.10; Xtensa GCC 14.2.0.
- Accepted production implementation compiled into a standalone device test.
- Sketch footprint: 310,412 bytes flash and 23,728 bytes static RAM including runtime, 1 KiB lookup oracle, 513-byte test buffer, and harness; not primitive-only.

## Device checks

- Empty input returns zero; null-data/zero-length accepted; null data/nonzero length and null output rejected; invalid data pointer preserves output sentinel.
- Standard vector `123456789` returns `0xCBF43926`.
- Exhaustive all 256 one-byte messages compared against an independently structured table-driven oracle.
- 100,000 deterministic random payloads with lengths 0..513, seed `0x524D0124`, differentially compared with the table oracle.
- Result: PASS; checksum `2918600907`.
- Fixed 32-byte payload: 25 batches of 10,000 calls, mean 1,748.34 cycles/call, range 1,748.25–1,748.40 cycles/call.
- Stress timing includes random payload generation and oracle checks; batch timing measures the production function.

## Raw serial result

```text
RMDEVICE|cv_crc32_compute|PASS|stress=100000|exhaustive=256|checksum=2918600907|stress_cycles=3982076361|batch=10000|min_cycles_x100=174825|mean_cycles_x100=174834|max_cycles_x100=174840|cpu_mhz=240
```

## Use and limitations

Useful when a packet, firmware block, Ethernet/ZIP-style file, or storage format specifies CRC-32/ISO-HDLC. CRC-32C uses a different polynomial. Hardware CRC or table lookup may improve throughput with peripheral or ROM cost; CRC does not provide authentication, and the caller controls serialization byte order.
