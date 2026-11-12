// login_test.cpp — login e2e for the Multistack prod deployment.
// Mirrors tests/e2e/LoginTests.java from the canonical suite.
#include <gtest/gtest.h>
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

// Fixture == TestBase: per-test fresh anonymous login page,
// url + screenshot on failure (Selenide screenshot-on-failure analogue).
class LoginTest : public testing::Test {
protected:
    static void SetUpTestSuite() {
        selenide::Configuration::webdriverUrl =
            envOr("WEBDRIVER_URL", "http://127.0.0.1:9515");
        selenide::Configuration::headless =
            envOr("HEADLESS", "true") != "false";
    }

    void SetUp() override {
        using namespace selenide;
        open(BASE + "/login");
        execute("localStorage.clear(); sessionStorage.clear()");
        open(BASE + "/login");
        $("[data-testid=login-form-title]")
            .shouldHave(have::exactText("Login Form"));
    }

    void TearDown() override {
        if (!HasFailure()) return;
        using namespace selenide;
        try {
            std::cerr << "url: "
                      << execute("return location.href").get<std::string>() << "\n";
            screenshot("fail.png");
            std::cerr << "screenshot: fail.png\n";
        } catch (...) {}
    }
};

TEST_F(LoginTest, shouldLoginWithValidCredentials) {
    using namespace selenide;
    $("[data-testid=login-input]").setValue(USER);
    $("[data-testid=password-input]").setValue(PASS);
    $("[data-testid=submit-button]").click();
    $("[data-testid=welcome-message]")
        .shouldHave(have::exactText("Welcome, " + USER + "!"));
}

TEST_F(LoginTest, shouldShowErrorWhenPasswordIsWrong) {
    using namespace selenide;
    $("[data-testid=login-input]").setValue(USER);
    $("[data-testid=password-input]").setValue("wrongpassword");
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::exactText("Wrong login or password"));
}
