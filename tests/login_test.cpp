// login_test.cpp — login e2e for the Multistack prod deployment
#include <gtest/gtest.h>
#include "selenide.hpp"
using namespace selenide;

TEST(LoginTest, wrongPassword) {
    open("https://autotests.ai/stack/backend-java-spring/frontend-typescript-react/login");
    $("[data-testid=login-input]").setValue("user1");
    $("[data-testid=password-input]").setValue("wrongpassword");
    $("[data-testid=submit-button]").click();
    $("[data-testid=error-message]")
        .shouldHave(have::exactText("Wrong login or password"));
    closeWebDriver();
}

TEST(LoginTest, validCredentials) {
    open("https://autotests.ai/stack/backend-java-spring/frontend-typescript-react/login");
    $("[data-testid=login-input]").setValue("user1");
    $("[data-testid=password-input]").setValue("password1");
    $("[data-testid=submit-button]").click();
    $("[data-testid=welcome-message]")
        .shouldHave(have::exactText("Welcome, user1!"));
    closeWebDriver();
}
