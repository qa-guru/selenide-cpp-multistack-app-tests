# selenide-cpp-multistack-app-tests

C++ e2e tests for the Multistack demo app, written on
[qa-guru/selenide-cpp](https://github.com/qa-guru/selenide-cpp) — consumed
here as a CMake dependency via `FetchContent` (tag `v0.1.0`).

Mirrors the canonical Java suite `tests-java-junit5-rest_assured-selenide`
(autotests-ai-multistack-app): register validation, duplicate username,
happy-path register → welcome, logout, login incl. wrong password,
delete account with `window.confirm`, localStorage token cleanup,
deleted-user login rejection.

## Stands

| Target | Deployment | Notes |
|--------|-----------|-------|
| `multistack` | [autotests.ai/stack/…-react](https://autotests.ai/stack/backend-java-spring/frontend-typescript-react) | prod SSOT build; `welcome-message`, `logout-button`, "Welcome,"/"Wrong login" |
| `new_app` | [new-app.qa.guru](https://new-app.qa.guru) | rotating copy ("Welcome,"/"Hey,"/"Hi,") — asserts the username, not the salutation |

Both create throwaway users and delete them inside the run.

## Run

```bash
chromedriver --port=9515 &      # any W3C WebDriver endpoint works
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

The first configure fetches `selenide-cpp` (and `nlohmann_json`, if not
installed) from GitHub — after that the build is offline-reproducible.

## Test flow (`multistack`)

```
register: empty submit -> "Login and password are required (minimum 3 and 6 characters)"
register: password mismatch -> "Passwords do not match"
register: duplicate username (seeded user1) -> "already taken" (API error)
register: happy path -> "Welcome, {user}!"
home: multistack-layout / items-list / health-status / session actions
logout -> "Login Form"
login: wrong password -> "Wrong login or password"
login: correct password -> welcome
delete account -> confirm "Delete this account? This cannot be undone."
   -> "Login Form" + auth key gone from localStorage
deleted user cannot login -> "Wrong login or password"
```

## License

MIT — see [LICENSE](LICENSE).
