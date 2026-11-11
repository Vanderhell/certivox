# Testing and validation

## API inventory and coverage

The 14 public declarations under `include/certivox/` each have a direct enabled test. `tests/test_audit_differential.c` independently checks all 14 APIs; the existing per-API programs remain enabled. All test checks use explicit `CHECK` logic and run in Debug and Release (`NDEBUG` does not disable them).

| API group | Direct verification |
|---|---|
| CRC-8/16/24/32/64 complete-buffer operations | 100,000 deterministic buffers per profile, table-driven references with a distinct byte-lookup implementation, standard known-answer vectors, empty input, null input/output and output preservation |
| CRC-32 byte update | 100,000 deterministic raw-state/byte/polynomial cases across 1,000 polynomials; table-driven oracle; standard streaming equivalence |
| CRC-32 combine | 100,000 deterministic A/B pairs checked against a direct table CRC of `A || B`; empty prefix/suffix, 65,536-byte suffix and an independent zlib 1.3.2 maximum-length vector |
| Polynomial reflection | 100,000 deterministic valid full-width inputs, invalid width/polynomial and output preservation; exhaustive all valid polynomial values for widths 1..8 |
| Hamming(7,4) encode/decode | All 16 payloads, all 128 received words against nearest-codeword oracle, all single-bit and 336 double-bit corruptions; double errors can miscorrect |
| Hamming(8,4) encode/decode | All 16 payloads and all 256 received bytes against nearest-codeword oracle, including every single-/double-bit corruption and uncorrectable-output preservation |
| SECDED(39,32) parity/check | 100,000 deterministic full-width words against a positional-mask parity reference; clean checks, 100,000 randomized singles and doubles; every 39 single and 741 double positions for five boundary/representative words |

The 32-bit data domains are not exhaustively enumerated. Deterministic differential testing and finite codeword-error coverage are evidence, not a formal proof.

## Local toolchain results

Strict warning flags are enabled with `CERTIVOX_STRICT_WARNINGS=ON`: GCC/Clang use `-Wall -Wextra -Wpedantic -Werror`; MSVC uses `/W4 /WX`. CMake sets `C_STANDARD 99` and disables extensions for library and tests. GCC and Clang builds use an explicit C99 dialect; MSVC successfully compiles the C99 source subset, but CMake emits no `/std:c99` switch for MSVC.

| Compiler | Configuration | Result |
|---|---|---|
| GCC 16.1.0 (MinGW UCRT) | Debug | PASS, 16/16 CTest |
| GCC 16.1.0 (MinGW UCRT) | Release | PASS, 16/16 CTest |
| ClangCL 19.1.5 (Visual Studio 17 2022) | Debug | PASS, 16/16 CTest |
| ClangCL 19.1.5 (Visual Studio 17 2022) | Release | PASS, 16/16 CTest |
| MSVC 19.42.34444 | Debug | PASS, 16/16 CTest |
| MSVC 19.42.34444 | Release | PASS, 16/16 CTest |
| GCC ASan (local Windows MinGW) | NOT RUN: linker cannot find `-lasan` |
| GCC UBSan (local Windows MinGW) | NOT RUN: linker cannot find `-lubsan` |

The MinGW sanitizer attempts failed during compiler link checks because the runtimes are absent; no sanitizer result is claimed locally. The remote GitHub Actions matrix is required for sanitizer evidence.

## Remote GitHub Actions

Workflow [43015999514](https://github.com/Vanderhell/certivox/actions/runs/43015999514) on commit `f758fded8e2db92df42deeda88a72bdfcd5be496` completed **8/8 PASS**: GCC Debug/Release, Clang Debug/Release, MSVC Debug/Release, GCC ASan and GCC UBSan. This run includes the new differential suite and supplies sanitizer evidence unavailable from the local MinGW installation.

## Reproduce

Single-configuration GCC/Clang build:

```sh
cmake -S . -B build -DCMAKE_C_COMPILER=gcc -DCMAKE_BUILD_TYPE=Release -DCERTIVOX_BUILD_TESTS=ON -DCERTIVOX_STRICT_WARNINGS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

For Visual Studio multi-configuration builds, pass the same configuration to both build and CTest:

```powershell
cmake -S . -B build -DCERTIVOX_BUILD_TESTS=ON -DCERTIVOX_STRICT_WARNINGS=ON
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

## ESP32-S3 status

**HW NOT VERIFIED for this audit.** The read-only Windows port inventory found Espressif USB serial devices at COM36, COM19 and COM37. COM37 is reserved; COM19 and COM36 may contain another project's firmware/data. Ports were enumerated by USB descriptor only; none were opened, reset, flashed or otherwise modified. No available board was authorized for Certivox, so no new hardware PASS or current performance claim is made.

Existing records under `docs/hardware/` and the extracted sketch run are historical evidence. The individual source records identify COM36 and report per-API tests; the old extracted integration run is recorded as COM37, 240 MHz, with 10,000 seeded CRC32 cases plus exhaustive Hamming/SECDED cases. Neither is a newly executed run or current-device verification. Hardware source copies under `hardware/esp32s3/certivox_hw/impl/` currently match their corresponding `src/*.c` files byte-for-byte, but that source match does not make the old firmware run current.

## Known limits

- Host differential sampling is not exhaustive over all input lengths and data values.
- The maximum CRC-combine suffix length has an independent fixed reference vector; no 4 GiB payload was materialized.
- Ordinary Hamming(7,4) can miscorrect multi-bit errors. Extended Hamming and SECDED guarantees cover only one- and two-bit error patterns respectively.
- CRC and ECC here are not cryptographic authentication or tamper protection.
- Current hardware cycles, per-module flash/RAM and stack use are not measured.
