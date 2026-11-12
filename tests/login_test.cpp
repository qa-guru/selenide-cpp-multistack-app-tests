// login_test.cpp — login e2e for the Multistack prod deployment.
// Two tests: valid credentials land on home, wrong password shows an error.
#include "selenide.hpp"

static std::string envOr(const char* key, const char* fallback) {
    const char* v = std::getenv(key);
    return (v && *v) ? v : fallback;
}

// prod stand by default; override via env to point at another deployment
static const std::string BASE = envOr(
    "BASE_URL",
    "https://autotests.ai/stack/backend-java-spring/frontend-typescript-react");
static const std::string USER = envOr("SEEDED_USER", "user1");
static const std::string PASS = envOr("SEEDED_PASSWORD", "password1");

static void dumpState(const char* where) {
    using namespace selenide;
    try {
        std::cerr << "--- " << where << " ---\n";
        std::cerr << "url: " << execute("return location.href").get<std::string>() << "\n";
        screenshot("fail.png");
        std::cerr << "screenshot: fail.png\n";
    } catch (...) {}
}

static void step(const char* name) { std::cout << "[step] " << name << "\n"; }

int main() {
    using namespace selenide;
    Configuration::webdriverUrl = envOr("WEBDRIVER_URL", "http://127.0.0.1:9515");
    Configuration::headless = envOr("HEADLESS", "true") != "false";

    try {

    step("user can login with valid credentials");
    open(BASE + "/login");
    $("[data-testid=login-input]").setValue(USER);
    $("[data-testid=password-input]").setValue(PASS);
    $("[data-testid=submit-button]").click();
    $("[data-testid=welcome-message]")
        .shouldHave(have::exactText("Welcome, " + USER + "!"));

    step("wrong password shows readable error");
    execute("localStorage.clear()");
    open(BASE + "/login");
    $("[data-testid=login-input]").setValue(USER);
    $("[data-testid=password-input]").setValue("wrongpassword");
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::exactText("Wrong login or password"));

    std::cout << "PASS\n";
    return 0;

    } catch (const std::exception& e) {
        dumpState(e.what());
        return 1;
    }
}
