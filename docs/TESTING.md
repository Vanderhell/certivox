# Testing

## Test summary

| Measure | Result |
|---|---:|
| Original source test programs migrated | 14 |
| New integration tests | 1 |
| Host test executables | 15 |
| Host result | PASS (reference build, see release record) |
| Hardware evidence packages | 14 |
| Extracted hardware run | PASS, 10,000 randomized CRC32 cases plus exhaustive error cases |
| Newly added tests | 1 |

## Test category table

| Category | Count | Platform | Result | Evidence |
|---|---:|---|---|---|
| CRC known vectors and invalid pointers | 5 | Host C99 | PASS | `tests/test_*crc*_compute.c` |
| CRC streaming/combine/reflection | 3 | Host C99 | PASS | `tests/test_crc_update_byte_u32.c`, `test_crc_combine_u32.c`, `test_crc_reflect_poly_u32.c` |
| Hamming exhaustive and error patterns | 4 | Host C99 | PASS | `tests/test_hamming*.c` |
| SECDED exhaustive/error patterns | 2 | Host C99 | PASS | `tests/test_secded*.c` |
| Cross-module integration | 1 | Host C99 | PASS | `tests/test_integration.c` |
| ESP32-S3 source evidence | 14 | ESP32-S3, 240 MHz | PASS in source records | `docs/hardware/*.md` |
| Extracted ESP32-S3 integration run | 1 run, 10,000 randomized CRC32 cases plus exhaustive Hamming/SECDED cases | ESP32-S3 QFN56 rev0.2, 240 MHz | PASS | `hardware/esp32s3/certivox_hw/certivox_hw.ino` |

## Tested functions

All 14 public functions are exercised by the migrated tests. Coverage includes
known vectors, null/invalid output pointers, empty input, exhaustive small
domains, single-bit correction, double-bit rejection, reserved-bit rejection,
CRC concatenation and deterministic seeded stress cases as reported by the
source records.

The integration test is new and only checks composition; it is not a substitute
for the per-function tests.

## Reproducibility

```sh
cmake -S . -B build -DCERTIVOX_BUILD_TESTS=ON -DCERTIVOX_BUILD_EXAMPLES=ON
cmake --build build --config Release
ctest --test-dir build --output-on-failure -C Release
```

For a warning-focused GCC build:

```sh
cmake -S . -B build-gcc -DCMAKE_C_COMPILER=gcc
cmake --build build-gcc
ctest --test-dir build-gcc --output-on-failure
```

## Test environment

| Environment | Result |
|---|---|
| Windows host, MSVC 19.42.34444, Visual Studio 17 2022, Release | 15/15 PASS |
| Windows host, GCC 16.1.0, Ninja, default release flags | 15/15 PASS |
| Windows host, ClangCL 19.1.5 with Visual Studio 17 2022, Release | 15/15 PASS |
| Windows host, GCC sanitizer build | NOT TESTED: MinGW linker could not find `libasan` and `libubsan` |
| ESP32-S3 extracted firmware | PASS; COM37, 240 MHz, ESP32 core 3.3.10, 10,000 randomized CRC32 cases plus exhaustive Hamming/SECDED cases |

## Known gaps

- Sanitizer runs are not yet recorded in this reference extraction.
- GNU-mode `clang.exe` is not the supported Windows configuration; use the
  Visual Studio `ClangCL` toolset so the MSVC runtime libraries are available.
- The extracted ESP32-S3 result is a real firmware run on COM37. Its reported
  footprint includes Arduino/runtime and the test harness; the A/B isolated
  delta is documented separately in `PERFORMANCE.md`.
- Stack usage and sanitizer results remain unverified.
- The product license is Apache-2.0 with copyright attribution to Vanderhell.
