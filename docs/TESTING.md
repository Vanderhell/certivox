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

Workflow [43016178113](https://github.com/Vanderhell/certivox/actions/runs/43016178113) on commit `48d7eeef414b7869358ffcf889949cd1e206106f` completed **8/8 PASS**: GCC Debug/Release, Clang Debug/Release, MSVC Debug/Release, GCC ASan and GCC UBSan. This run includes the differential suite and supplies sanitizer evidence unavailable from the local MinGW installation.

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

**Current hardware result: PASS, 14/14 APIs** on the user-authorized ESP32-S3 at COM37 (USB Serial/JTAG; MAC `3c:0f:02:d9:77:30`; ESP32-S3 revision 0.2). Validation firmware was built with ESP-IDF 5.5.1 and all 14 public APIs were called and checked against known answers, independent positional/parity references, and exhaustive Hamming/SECDED bit-error patterns for representative data. Serial output: `CERTIVOX_ALL14_PASS|passed=14/14|first_failed_api=0`.

Only app0 (`0x10000..0x3FFFF`) was written. No bootloader, partition table, OTA metadata, NVS, app1, SPIFFS or coredump areas were written. Per the user's instruction, no backup was made; the prior app0 contents were overwritten and were not restored. COM19 and COM36 were not used. This is an explicit test deployment rather than a nondestructive inspection.

The first firmware run failed the API 13 check because the temporary, independent SECDED parity oracle initialized overall parity from the data word and then counted each set data bit again. The oracle was corrected and the rebuilt firmware then passed all 14 checks. This was a test-harness defect; no production source was changed.

Earlier records under `docs/hardware/` and the extracted sketch run remain historical evidence and are not used as this run's result. The current firmware compiled the production `src/*.c` files directly as C99. Host and hardware checks are independent evidence; hardware vectors are targeted and do not exhaust 32-bit data domains.

## Known limits

- Host differential sampling is not exhaustive over all input lengths and data values.
- The maximum CRC-combine suffix length has an independent fixed reference vector; no 4 GiB payload was materialized.
- Ordinary Hamming(7,4) can miscorrect multi-bit errors. Extended Hamming and SECDED guarantees cover only one- and two-bit error patterns respectively.
- CRC and ECC here are not cryptographic authentication or tamper protection.
- The hardware run uses representative vectors; it does not exhaust every CRC input or the complete SECDED data domain.
- Hardware performance and footprint measurement method and limitations are recorded in `docs/PERFORMANCE.md`.
