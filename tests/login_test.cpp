// login_test.cpp — login e2e for the Multistack prod deployment
#include <gtest/gtest.h>
#include "selenide.hpp"
using namespace selenide;

static const std::string BASE = getenv("BASE_URL") ? getenv("BASE_URL") :
    "https://autotests.ai/stack/backend-java-spring/frontend-typescript-react";

static void openLogin() {
    open(BASE + "/login");
    execute("localStorage.clear()");
    open(BASE + "/login");  // reload so the cleared token can't auto-login
}

TEST(LoginTest, validCredentials) {
    openLogin();
    $("[data-testid=login-input]").setValue("user1");
    $("[data-testid=password-input]").setValue("password1");
    $("[data-testid=submit-button]").click();
    $("[data-testid=welcome-message]")
        .shouldHave(have::exactText("Welcome, user1!"));
}

TEST(LoginTest, wrongPassword) {
    openLogin();
    $("[data-testid=login-input]").setValue("user1");
    $("[data-testid=password-input]").setValue("wrongpassword");
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::exactText("Wrong login or password"));
}
