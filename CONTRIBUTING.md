# Contributing

Contributions should preserve the documented C99 API contracts, deterministic
behavior, caller ownership of buffers and state, and the absence of mandatory
heap or operating-system dependencies.

Before proposing changes, run the strict CMake build and CTest suite in both
Debug and Release. Add an enabled regression test for each behavior change;
checks must not depend on `assert()` because Release builds may define
`NDEBUG`. Keep reference implementations independent from production code.

Use a focused pull request with a description of the affected APIs, behavior,
test commands and any compiler or hardware configurations not run. Do not add
unmeasured performance claims.
