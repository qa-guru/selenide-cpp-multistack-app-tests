# selenide-cpp-multistack-app-tests

C++ e2e tests for the Multistack demo app on
[qa-guru/selenide-cpp](https://github.com/qa-guru/selenide-cpp) —
Google Test + `FetchContent` dependency (`v0.1.2`).

## Run

```bash
chromedriver --port=9515 &
cmake -B build && cmake --build build && ./build/login_test
```

Tests run against
[autotests.ai](https://autotests.ai/stack/backend-java-spring/frontend-typescript-react/login).

## License

MIT — see [LICENSE](LICENSE).
