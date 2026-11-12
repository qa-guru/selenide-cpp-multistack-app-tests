// multistack.cpp — e2e against autotests-ai-multistack-app prod deployment
//   https://autotests.ai/stack/backend-java-spring/frontend-typescript-react
//
// Mirrors the canonical suite tests-java-junit5-rest_assured-selenide:
//   RegisterTests (validation, duplicate, happy path) + LoginTests (wrong
//   password) + DeleteAccountTests (confirm + localStorage token cleanup).
#include "selenide.hpp"

static const std::string BASE =
    "https://autotests.ai/stack/backend-java-spring/frontend-typescript-react";

// скриншот + url на падении — аналог Selenide screenshot-on-failure
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

    const std::string user =
        "cpp_" + std::to_string(std::chrono::steady_clock::now()
                                    .time_since_epoch().count() % 100000000);
    const std::string pass = "password123";

    try {

    // RegisterTests: пустой submit -> both-required валидация
    step("register: empty submit shows both-required error");
    open(BASE + "/register");
    $("[data-testid=register-submit-button]").click();
    $("[data-testid=register-error-message]").shouldHave(have::exactText(
        "Login and password are required (minimum 3 and 6 characters)"));

    // RegisterTests: пароли не совпадают
    step("register: password mismatch");
    $("[data-testid=register-login-input]").setValue(user);
    $("[data-testid=register-password-input]").setValue(pass);
    $("[data-testid=confirm-password-input]").setValue(pass + "x");
    $("[data-testid=register-submit-button]").click();
    $("[data-testid=register-error-message]")
        .shouldHave(have::exactText("Passwords do not match"));

    // RegisterTests: дубликат username -> API-ошибка (user1 засеян на проде)
    step("register: duplicate username -> api error");
    $("[data-testid=register-login-input]").setValue("user1");
    $("[data-testid=confirm-password-input]").setValue(pass);
    $("[data-testid=register-submit-button]").click();
    $("[data-testid=register-error-message]")
        .shouldHave(have::text("already taken"));

    // RegisterTests: happy path -> auto-login -> home
    step("register: happy path lands on home");
    $("[data-testid=register-login-input]").setValue(user);
    $("[data-testid=register-submit-button]").click();
    $("[data-testid=welcome-panel]").shouldBe(be::visible);
    $("[data-testid=welcome-message]")
        .shouldHave(have::exactText("Welcome, " + user + "!"));

    // HomeTests: layout + health + session actions
    step("home: layout, health, session actions");
    $("[data-testid=multistack-layout]").shouldBe(be::visible);
    $("[data-testid=items-list]").shouldBe(be::visible);
    $("[data-testid=health-status]").shouldHave(have::text("service:"));
    $("[data-testid=logout-button]").shouldHave(have::exactText("Logout"));
    $("[data-testid=delete-account-button]")
        .shouldHave(have::exactText("Delete account"));

    // LogoutTests: logout -> Login Form
    step("logout navigates to login form");
    $("[data-testid=logout-button]").click();
    $("[data-testid=login-form-title]").shouldHave(have::exactText("Login Form"));

    // LoginTests: неверный пароль -> Wrong login or password
    step("login: wrong password -> error");
    $("[data-testid=login-input]").setValue(user);
    $("[data-testid=password-input]").setValue(pass + "x");
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::exactText("Wrong login or password"));

    // LoginTests: верный пароль -> welcome
    step("login: correct password -> welcome");
    $("[data-testid=password-input]").setValue(pass);
    $("[data-testid=submit-button]").click();
    $("[data-testid=welcome-message]").shouldHave(have::text(user));

    // DeleteAccountTests: confirm -> login form + authToken cleared
    step("delete account: confirm clears session");
    $("[data-testid=delete-account-button]").click();
    if (alertText() != "Delete this account? This cannot be undone.")
        throw std::runtime_error("unexpected confirm text: " + alertText());
    acceptAlert();
    $("[data-testid=login-form-title]").shouldHave(have::exactText("Login Form"));
    json authKeys = execute(
        "return Object.keys(localStorage).filter(k => /auth/i.test(k))");
    if (!authKeys.is_array() || !authKeys.empty())
        throw std::runtime_error("auth token not cleared: " + authKeys.dump());

    // финал: аккаунт удалён — повторный login падает
    step("deleted user cannot login");
    $("[data-testid=login-input]").setValue(user);
    $("[data-testid=password-input]").setValue(pass);
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::exactText("Wrong login or password"));

    std::cout << "PASS (user " << user << ")\n";
    return 0;

    } catch (const std::exception& e) {
        dumpState(e.what());
        return 1;
    }
}
