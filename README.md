# selenide-cpp-multistack-app-tests

C++ e2e tests for the Multistack demo app on
[qa-guru/selenide-cpp](https://github.com/qa-guru/selenide-cpp) — consumed
as a CMake dependency via `FetchContent` (tag `v0.1.1`).

## Tests

| File | Cases (each `TEST_F` is its own CTest test) |
|------|---------------------------------------------|
| `tests/login_test.cpp` | `shouldLoginWithValidCredentials`, `shouldShowErrorWhenPasswordIsWrong` |

Tests are written on **Google Test** (fetched at configure time): a fixture
(`LoginTest`) plays `TestBase` — fresh anonymous login page per test,
url + screenshot on failure. Every `tests/*_test.cpp` becomes a binary and
`gtest_discover_tests` registers each `TEST_F` as a separate CTest case, so a
run reports per-method results:

```
1/2 Test #1: LoginTest.shouldLoginWithValidCredentials ...... Passed
2/2 Test #2: LoginTest.shouldShowErrorWhenPasswordIsWrong ... Passed
```

## Configuration

Prod stand by default; override via environment:

| Var | Default |
|-----|---------|
| `BASE_URL` | `https://autotests.ai/stack/backend-java-spring/frontend-typescript-react` |
| `WEBDRIVER_URL` | `http://127.0.0.1:9515` |
| `HEADLESS` | `true` |
| `SEEDED_USER` / `SEEDED_PASSWORD` | `user1` / `password1` (seeded account) |

## Run

```bash
chromedriver --port=9515 &
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

## License

MIT — see [LICENSE](LICENSE).
