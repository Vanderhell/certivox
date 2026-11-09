# Certivox

## Overview

Certivox is a portable C99 library for deterministic integrity checks and
small error-correcting codes in embedded binary data paths.

## The problem

Firmware often needs to detect corrupted frames, protect compact words, or
correct a small number of bit errors without heap allocation, an operating
system, or a large protocol stack.

## Why this product exists

The included CRC and Hamming/SECDED primitives form a coherent data-integrity
boundary. They are stateless, caller-buffered operations that can be selected
one source file at a time.

## Features

- CRC-8, CRC-16/MODBUS, CRC-24/OpenPGP, CRC-32/ISO-HDLC and CRC-64/ECMA-182.
- Reflected CRC-32 byte update and CRC-32 concatenation.
- CRC polynomial bit reflection.
- Hamming(7,4), extended Hamming(8,4), and SECDED Hamming(39,32).
- Explicit status values and unchanged outputs on invalid/uncorrectable input.
- No heap, global mutable state, threads or OS dependencies.

## Architecture

The public headers are in `include/certivox/`; implementations are split by
primitive in `src/`. The static CMake target is convenient for full tests, but
individual source files can be compiled directly for a smaller firmware.
See [the architecture document](docs/ARCHITECTURE.md).

## Supported platforms

The code uses standard C99 integer and size types and is intended for bare
metal and hosted C environments. The source validation records cover ESP32-S3
at 240 MHz. A post-extraction ESP32-S3 run is pending.

## Installation

```sh
cmake -S . -B build -DCERTIVOX_BUILD_TESTS=OFF
cmake --build build --config Release
```

Link `certivox` and add `include/` to the compiler include path. For minimal
integration, compile only the required `src/cv_*.c` files.

## Quick start

```c
#include <certivox/cv_crc32_compute.h>
#include <stdint.h>
#include <stddef.h>

uint32_t frame_crc(const uint8_t *frame, size_t length)
{
    uint32_t crc = 0u;
    return cv_crc32_compute(frame, length, &crc) ? crc : 0u;
}
```

## API reference

The complete public API is the 14 headers under `include/certivox/`. CRC
functions reject a null data pointer when `length != 0`; output pointers must
be non-null. Hamming and SECDED decoders document their correction and
uncorrectable statuses in their headers. Integer arithmetic is unsigned and
well-defined modulo the declared width; no dynamic allocation is performed.

## Configuration and dependencies

There are no mandatory third-party dependencies or runtime configuration
options. CMake options are `CERTIVOX_BUILD_TESTS` and
`CERTIVOX_BUILD_EXAMPLES`. Consumers can omit CMake and compile selected
sources directly.

## Memory footprint and performance

No isolated flash, RAM or stack measurement is claimed yet. The inherited
ESP32-S3 records contain harness-inclusive measurements and are summarized in
[PERFORMANCE.md](docs/PERFORMANCE.md). The host benchmark is not an MCU
measurement.

## Testing and hardware validation

Fourteen source test programs plus one new integration test are provided. Run
them with CTest; details and known gaps are in [TESTING.md](docs/TESTING.md).
The source records report PASS on ESP32-S3, but this extracted product is
classified `EXPERIMENTAL` until it has a fresh hardware run, isolated footprint
measurements and license clearance.

## Limitations and security

CRC detects accidental corruption and is not authentication. Hamming and
SECDED contracts only cover the error patterns documented in their headers.
CRC profiles are not interchangeable. This library does not provide secrecy,
key management or tamper resistance.

## Versioning and license

Version `0.1.0` is an extraction baseline. Numerical behavior follows the
source implementations; API compatibility will be maintained within a major
version after release. No source license file was available, so this staging
repository is not cleared for redistribution; see `LICENSE`.

## Origin

Certivox is extracted from PrimitiveCore's `encoding/wire_and_binary` family,
which preserves validated implementations originating in the RoundMath source
repository. The standalone `cv_` prefix is a product-level API adaptation;
the algorithms and numeric contracts were not intentionally changed.
