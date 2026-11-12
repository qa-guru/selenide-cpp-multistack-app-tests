# selenide-cpp-multistack-app-tests

Two Google Test login checks against
[autotests.ai](https://autotests.ai/stack/backend-java-spring/frontend-typescript-react/login):
wrong password and valid `user1` / `password1` credentials.
One local lifecycle check verifies cleanup after session creation fails.

Requires macOS/Linux, CMake 3.20+, C++17, libcurl headers/library, Chrome and a
matching ChromeDriver in `PATH`. The framework starts/stops its own driver;
no separate ChromeDriver server is needed.
Uses [selenide-cpp](https://github.com/qa-guru/selenide-cpp) `v0.1.3`
and Google Test `v1.15.2` through CMake FetchContent.

First setup:

```bash
cmake -B build             # configure/generate build/, not compile
```

Build and run after setup or after ordinary C++ edits:

```bash
cmake --build build && ./build/login_test
```

In `--build build`, `--build` is the action; `build` is the directory name.

## CLion

Open this directory as a CMake project, wait for configuration, then use Run/Debug
beside any `TEST(...)`. Ensure ChromeDriver's directory is in the run configuration's
`PATH`; the IDE does not need a separate driver launch configuration.

To test local framework changes, add
`-DFETCHCONTENT_SOURCE_DIR_SELENIDE=/path/to/selenide-cpp` to `cmake -B build`.

MIT, see [LICENSE](LICENSE).
