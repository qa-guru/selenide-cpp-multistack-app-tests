# selenide-cpp-multistack-app-tests

C++ e2e tests for the Multistack demo app on
[qa-guru/selenide-cpp](https://github.com/qa-guru/selenide-cpp) — consumed
as a CMake dependency via `FetchContent` (tag `v0.1.0`).

A line-by-line port of the canonical Java suite
`tests-java-junit5-rest_assured-selenide` (autotests-ai-multistack-app):
same Page Objects, same test classes, same expected copy, same API-driven
setup/cleanup for throwaway users.

## Layout

```
tests/
├── support/
│   ├── test_base.hpp    — env config (prod defaults) + case runner + DataFaker
│   ├── api_client.hpp   — /auth/* API for user setup/cleanup (AuthApiClient)
│   └── pages.hpp        — LoginPage / RegisterPage / HomePage / BasePage
├── login_test.cpp           ← LoginTests    (9 cases)
├── register_test.cpp        ← RegisterTests (9 cases)
├── logout_test.cpp          ← LogoutTests   (2 cases)
├── home_test.cpp            ← HomeTests     (smoke)
├── session_test.cpp         ← SessionTests  (2 cases)
└── delete_account_test.cpp  ← DeleteAccountTests (2 cases)
```

Each `*_test.cpp` is one CTest target; new files are picked up by glob.

## Configuration

All defaults point at the prod stand — override via environment
(mirrors `config/prod.properties` + `TestConfig`):

| Var | Default | Meaning |
|-----|---------|---------|
| `BASE_URL` | `https://autotests.ai/stack/backend-java-spring/frontend-typescript-react` | app under test |
| `API_BASE_URL` | `https://autotests.ai/stack/backend-java-spring/api` | auth API origin (`{backend}/api`) |
| `WEBDRIVER_URL` | `http://127.0.0.1:9515` | W3C WebDriver endpoint |
| `BROWSER` | `chrome` | `browserName` capability |
| `HEADLESS` | `true` | `--headless=new` |
| `TIMEOUT_MS` | `5000` | auto-wait timeout |
| `HEALTH_SERVICE` | `backend-java-spring` | expected id in the health panel |
| `SEEDED_USER` / `SEEDED_PASSWORD` | `user1` / `password1` | seeded account (read-only) |

## Run

```bash
chromedriver --port=9515 &      # any W3C WebDriver endpoint
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
# one suite only:
ctest --test-dir build -R login_test --output-on-failure
```

The first configure fetches `selenide-cpp` (and `nlohmann_json` if not
installed) from GitHub — after that the build is offline-reproducible.

Throwaway users are created via `POST /auth/register` and deleted via
`DELETE /auth/me` (`UserCleanup`, RAII `@AfterEach`) — the stand stays clean
even when a case fails mid-way.

## License

MIT — see [LICENSE](LICENSE).
