# selenide-cpp-multistack-app-tests

C++ e2e tests for the Multistack demo app on
[qa-guru/selenide-cpp](https://github.com/qa-guru/selenide-cpp) — consumed
as a CMake dependency via `FetchContent` (tag `v0.1.1`).

## Tests

| File | Cases |
|------|-------|
| `tests/login_test.cpp` | valid credentials → welcome panel; wrong password → "Wrong login or password" |

Every `tests/*_test.cpp` becomes a CTest target automatically.

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
