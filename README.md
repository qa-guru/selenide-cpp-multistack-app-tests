# selenide-cpp-multistack-app-tests

C++ e2e tests for the Multistack demo app on
[qa-guru/selenide-cpp](https://github.com/qa-guru/selenide-cpp) —
Google Test + `FetchContent` dependency (`v0.1.1`).

## Run

```bash
chromedriver --port=9515 &
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

Each `TEST` registers as its own CTest case. Tests hit the prod deployment.
New suite = new `tests/*_test.cpp`.

## License

MIT — see [LICENSE](LICENSE).
