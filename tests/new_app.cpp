// new_app.cpp — e2e against https://new-app.qa.guru (Multistack demo, EN build)
//
// Journey: form validation -> register -> welcome -> logout -> login ->
//          delete account (window.confirm) -> deleted user cannot login.
// Selectors follow the live deployment: welcome-title, sign-out-button,
// delete-account-button, health-status (differs from SSOT sources).
// Greeting copy rotates on this stand ("Welcome," / "Hey," / "Hi,") —
// assert the username, not the salutation word.
#include "selenide.hpp"

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

int main() {
    using namespace selenide;

    const std::string user =
        "cpp_" + std::to_string(std::chrono::steady_clock::now()
                                    .time_since_epoch().count() % 100000000);
    const std::string pass = "qwerty123";

    try {

    // 1. login-форма: пустой submit -> клиентская валидация
    open("https://new-app.qa.guru/login");
    $("[data-testid=login-form-title]")
        .shouldBe(be::visible)
        .shouldHave(have::exactText("Login Form"));
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::text("required"));

    // 2. переход на register по ссылке
    $("[data-testid=register-link]").click();
    $("[data-testid=register-form-title]").shouldHave(have::exactText("Register"));

    // 3. клиентская валидация: пароли не совпадают
    $("[data-testid=register-login-input]").setValue(user);
    $("[data-testid=register-password-input]").setValue(pass);
    $("[data-testid=confirm-password-input]").setValue("other123");
    $("[data-testid=register-submit-button]").click();
    $("[data-testid=register-error-message]")
        .shouldHave(have::exactText("Passwords do not match"));

    // 4. настоящая регистрация -> auto-login -> домашняя страница
    $("[data-testid=confirm-password-input]").setValue(pass);
    $("[data-testid=register-submit-button]").click();
    $("[data-testid=welcome-title]")
        .shouldHave(have::text(user + "!"));
    $("[data-testid=health-status]").shouldHave(have::text("service:"));
    $("[data-testid=multistack-layout]").shouldBe(be::exist);

    // 5. logout -> повторный login тем же юзером
    $("[data-testid=sign-out-button]").click();
    $("[data-testid=login-form]").shouldBe(be::visible);
    $("[data-testid=login-input]").setValue(user);
    $("[data-testid=password-input]").setValue(pass);
    $("[data-testid=submit-button]").click();
    $("[data-testid=welcome-title]").shouldHave(have::text(user));

    // 6. delete account — window.confirm через WebDriver alert API (cleanup)
    $("[data-testid=delete-account-button]").click();
    if (alertText().find("Delete this account") == std::string::npos)
        throw std::runtime_error("unexpected confirm text: " + alertText());
    acceptAlert();
    $("[data-testid=login-form]").shouldBe(be::visible);

    // 7. аккаунт удалён — повторный login обязан упасть
    $("[data-testid=login-input]").setValue(user);
    $("[data-testid=password-input]").setValue(pass);
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::text("login or password"));

    std::cout << "PASS (user " << user << ")\n";
    return 0;

    } catch (const std::exception& e) {
        dumpState(e.what());
        return 1;
    }
}
