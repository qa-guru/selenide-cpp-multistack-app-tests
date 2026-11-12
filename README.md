# selenide-cpp-multistack-app-tests

Two Google Test login checks against
[autotests.ai](https://autotests.ai/stack/backend-java-spring/frontend-typescript-react/login):
wrong password and valid `user1` / `password1` credentials.

Requires CMake 3.20+, C++17, libcurl headers/library and Chrome/ChromeDriver.
Uses [selenide-cpp](https://github.com/qa-guru/selenide-cpp) `v0.1.2`
and Google Test `v1.15.2` through CMake FetchContent.

```bash
chromedriver --port=9515 &
cmake -B build && cmake --build build && ./build/login_test
```

To test local framework changes, add
`-DFETCHCONTENT_SOURCE_DIR_SELENIDE=/path/to/selenide-cpp` to `cmake -B build`.

MIT, see [LICENSE](LICENSE).
