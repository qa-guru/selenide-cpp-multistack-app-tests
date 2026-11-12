// test_base.hpp — suite plumbing for the Multistack e2e port.
// Mirrors tests/TestBase.java + config/ + helpers/DataFaker.java of the
// canonical module tests-java-junit5-rest_assured-selenide: env-driven
// config (prod stand by default) and throwaway user identities.
#pragma once

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "selenide.hpp"

namespace config {

inline std::string envOr(const char* key, const char* fallback) {
    const char* v = std::getenv(key);
    return (v && *v) ? v : fallback;
}

// Defaults = prod stand (mirrors config/prod.properties of the Java suite).
inline const std::string baseUrl = envOr(
    "BASE_URL",
    "https://autotests.ai/stack/backend-java-spring/frontend-typescript-react");
// Frontend resolves API as {backend mount}/api — see lib/appBase.ts apiUrl().
inline const std::string apiBaseUrl = envOr(
    "API_BASE_URL", "https://autotests.ai/stack/backend-java-spring/api");
inline const std::string webdriverUrl = envOr("WEBDRIVER_URL", "http://127.0.0.1:9515");
inline const std::string browser      = envOr("BROWSER", "chrome");
inline const bool        headless     = envOr("HEADLESS", "true") != "false";
inline const int         timeoutMs    = std::stoi(envOr("TIMEOUT_MS", "5000"));
// Mirrors TestConfig.apiHealthService — id shown by the health panel.
inline const std::string healthService  = envOr("HEALTH_SERVICE", "backend-java-spring");
// Seeded prod account (fixtures) — read-only, never deleted by tests.
inline const std::string seededUser     = envOr("SEEDED_USER", "user1");
inline const std::string seededPassword = envOr("SEEDED_PASSWORD", "password1");

inline void apply() {
    selenide::Configuration::webdriverUrl = webdriverUrl;
    selenide::Configuration::browser      = browser;
    selenide::Configuration::headless     = headless;
    selenide::Configuration::timeout      = std::chrono::milliseconds(timeoutMs);
}

} // namespace config

// Canonical UI copy — the strings the Java suite pins (RegisterTests /
// LoginTests constants and HomePage DELETE_ACCOUNT_CONFIRM).
namespace msg {
inline const std::string bothRequired =
    "Login and password are required (minimum 3 and 6 characters)";
inline const std::string loginRequired    = "Login is required (minimum 3 characters)";
inline const std::string loginMinLength   = "Login must be at least 3 characters";
inline const std::string passwordRequired = "Password is required (minimum 6 characters)";
inline const std::string passwordMinLength = "Password must be at least 6 characters";
inline const std::string passwordMismatch = "Passwords do not match";
inline const std::string duplicateUser    = "Username already taken";
inline const std::string wrongCredentials = "Wrong login or password";
inline const std::string deleteConfirm    = "Delete this account? This cannot be undone.";
} // namespace msg

// helpers/DataFaker.java — throwaway identities within backend @Size bounds.
namespace fake {
inline std::string hex(size_t n) {
    static std::mt19937_64 rng(std::random_device{}());
    static const char* digits = "0123456789abcdef";
    std::string s;
    for (size_t i = 0; i < n; ++i) s += digits[rng() % 16];
    return s;
}
inline std::string username()           { return "cpp_" + hex(8); }          // 3..64 chars
inline std::string password()           { return "pw" + hex(8); }            // >= 6 chars
inline std::string usernameAtMinLength(){ return hex(3); }                   // exactly 3
inline std::string passwordAtMinLength(){ return "123456"; }                 // exactly 6
} // namespace fake

// Minimal case runner: every case starts from a clean browser state
// (localStorage cleared — the C++ equivalent of closeBrowserAfterEach), and a
// failure marks the case without aborting the suite.
namespace suite {

inline void dumpState(const std::string& name) {
    using namespace selenide;
    try {
        std::string file = "fail";
        for (char c : name)
            file += std::isalnum(static_cast<unsigned char>(c)) ? c : '_';
        file += ".png";
        std::cerr << "    url: "
                  << execute("return location.href").get<std::string>() << "\n";
        json els = execute(
            "var out={};"
            "document.querySelectorAll('[data-testid]').forEach(e=>{"
            "  var k=e.dataset.testid;"
            "  if(/error|input|title/.test(k)) out[k]=(e.value!==undefined?e.value:e.textContent);"
            "});"
            "return out;");
        std::cerr << "    dom: " << els.dump() << "\n";
        screenshot(file);
        std::cerr << "    screenshot: " << file << "\n";
    } catch (...) {
    }
}

using Case = std::pair<std::string, std::function<void()>>;

inline int run(const std::string& name, std::initializer_list<Case> cases) {
    config::apply();
    int failed = 0;
    std::cout << "== " << name << " ==\n";
    for (auto& [caseName, fn] : cases) {
        try {
            selenide::open(config::baseUrl + "/login");
            selenide::execute("localStorage.clear(); sessionStorage.clear()");
            fn();
            std::cout << "  PASS " << caseName << "\n";
        } catch (const std::exception& e) {
            ++failed;
            std::cout << "  FAIL " << caseName << "\n    " << e.what() << "\n";
            dumpState(caseName);
        }
    }
    std::cout << name << ": " << (cases.size() - failed) << "/" << cases.size()
              << " passed\n";
    return failed ? 1 : 0;
}

} // namespace suite
