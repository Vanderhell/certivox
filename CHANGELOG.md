# Changelog

## 0.2.0 - product API namespace migration

**Breaking API change:** public identifiers and header basenames have moved from
`cv_*` to the unique `certivox_*` product namespace. For example,
`<certivox/cv_crc32_compute.h>` / `cv_crc32_compute` become
`<certivox/crc32_compute.h>` / `certivox_crc32_compute`. Status types and enum
constants use the corresponding `CERTIVOX_*` names. The complete old-to-new public function mapping is listed in
`docs/API_REFERENCE.md`. No CRC or error-correction algorithm was intentionally changed.

- Added the namespaced `Certivox::Certivox` CMake target and installable package.
- Made tests, examples and benchmarks opt-in for consuming projects.
- Added isolated C header compilation checks and C++ public-header smoke test.
- Retained original test/oracle and hardware validation inputs for auditability.
- Current ESP32-S3 validation for this revision remains HW NOT VERIFIED.
