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

Strict warnings are enabled by `CERTIVOX_STRICT_WARNINGS=ON`: GCC/Clang use `-Wall -Wextra -Wpedantic -Werror`; MSVC uses `/W4 /WX`. The production library and C tests compile as ISO C99 with extensions disabled. All checks are explicit `CHECK` logic and remain active in Release. The suite contains 16 C behavioral tests, 14 isolated C public-header compilation tests, and one aggregate C++ header/link smoke test (31 CTest entries).

| Compiler | Configuration | Result |
|---|---|---|
| GCC 16.1.0 MinGW UCRT | Debug | PASS, 31/31 CTest |
| GCC 16.1.0 MinGW UCRT | Release | PASS, 31/31 CTest |
| ClangCL 19.1.5 (Visual Studio 17 2022) | Debug | PASS, 31/31 CTest |
| ClangCL 19.1.5 (Visual Studio 17 2022) | Release | PASS, 31/31 CTest |
| MSVC 19.42.34444 | Debug | PASS, 31/31 CTest |
| MSVC 19.42.34444 | Release | PASS, 31/31 CTest |
| GCC ASan | NOT RUN: MinGW linker lacks `-lasan`; WSL process creation returned `E_ACCESSDENIED` |
| GCC UBSan | NOT RUN: MinGW linker lacks `-lubsan`; WSL process creation returned `E_ACCESSDENIED` |

The package-install `find_package(Certivox)` consumer, `add_subdirectory` consumer, and direct-source C99 example were configured, built and executed successfully with GCC 16.1.0. The example links the documented Hamming(7,4) helper source dependency. Historical remote CI below was run against a pre-productization revision and does not validate the current namespaced source revision.

## Historical remote GitHub Actions

Workflow [43016178113](https://github.com/Vanderhell/certivox/actions/runs/43016178113) on pre-productization commit `48d7eeef414b7869358ffcf889949cd1e206106f` completed 8/8 jobs. This is historical evidence only; it does not validate the present namespace/layout changes. Current-revision remote Actions have not run.

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

**Current product revision: HW NOT VERIFIED.** Historical validation reports recorded successful calls for all 14 pre-productization APIs on an ESP32-S3. Those runs predate the product namespace and file migration, and have not been repeated against this revision. The validation source is retained under `tests/hardware/esp32s3/`; no device was programmed as part of this audit pass.

The earlier firmware result included a corrected SECDED test-oracle defect; no production change was needed. Historical cycle and footprint values are identified separately in `docs/PERFORMANCE.md`. They are evidence for the earlier build configuration, not current-revision measurements.

## Known limits

- Host differential sampling is not exhaustive over all input lengths and data values.
- The maximum CRC-combine suffix length has an independent fixed reference vector; no 4 GiB payload was materialized.
- Ordinary Hamming(7,4) can miscorrect multi-bit errors. Extended Hamming and SECDED guarantees cover only one- and two-bit error patterns respectively.
- CRC and ECC here are not cryptographic authentication or tamper protection.
- Historical hardware vectors are representative; they do not exhaust every CRC input or the complete SECDED data domain.
- Hardware performance and footprint measurement method and limitations are recorded in `docs/PERFORMANCE.md`.
