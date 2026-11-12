# selenide-cpp-multistack-app-tests

Two Google Test login checks against
[autotests.ai](https://autotests.ai/stack/backend-java-spring/frontend-typescript-react/login):
wrong password and valid `user1` / `password1` credentials.

Requires CMake 3.20+, C++17, libcurl headers/library and Chrome/ChromeDriver.
Uses [selenide-cpp](https://github.com/qa-guru/selenide-cpp) `v0.1.2`
and Google Test `v1.15.2` through CMake FetchContent.

First setup:

```bash
chromedriver --port=9515 &  # start once; reuse if already running
cmake -B build             # configure/generate build/, not compile
```

Build and run after setup or after ordinary C++ edits:

```bash
cmake --build build && ./build/login_test
```

In `--build build`, `--build` is the action; `build` is the directory name.

To test local framework changes, add
`-DFETCHCONTENT_SOURCE_DIR_SELENIDE=/path/to/selenide-cpp` to `cmake -B build`.

MIT, see [LICENSE](LICENSE).
