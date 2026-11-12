// login_test.cpp — login e2e for the Multistack prod deployment
#include <gtest/gtest.h>
#include "selenide.hpp"
#ifndef _WIN32
#include <cerrno>
#include <sys/wait.h>
#endif
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

#ifndef _WIN32
TEST(DriverTest, failedSessionClosesDriver) {
    closeWebDriver();
    auto browser = Configuration::browser;
    Configuration::browser = "not-a-browser";
    EXPECT_THROW(open("about:blank"), std::runtime_error);
    Configuration::browser = browser;
    closeWebDriver();

    pid_t child = waitpid(-1, nullptr, WNOHANG);
    int error = errno;
    EXPECT_EQ(child, -1);
    EXPECT_EQ(error, ECHILD);
}
#endif

TEST(DriverTest, executeRunsJs) {
    EXPECT_EQ(execute("return arguments[0] + 1", {41}), 42);
    EXPECT_EQ(executeAsync("arguments[arguments.length-1]('ok')"), "ok");
    closeWebDriver();
}
