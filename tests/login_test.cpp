// login_test.cpp — port of tests/e2e/LoginTests.java.
#include "support/pages.hpp"

int main() {
    using pages::LoginPage;
    return suite::run("LoginTests", {
        {"shouldLoginWithValidCredentials", [] {
            LoginPage{}.openPage()
                .fillAndSubmitForm(config::seededUser, config::seededPassword)
                .shouldHaveWelcomeMessage("Welcome, " + config::seededUser + "!");
        }},
        {"shouldLoginWithMinimumLengthCredentials", [] {
            UserCleanup user{fake::usernameAtMinLength(), fake::passwordAtMinLength()};
            api::reg(user.user, user.pass);
            LoginPage{}.openPage()
                .fillAndSubmitForm(user.user, user.pass)
                .shouldHaveWelcomeMessage("Welcome, " + user.user + "!");
        }},
        {"shouldShowValidationErrorWhenUsernameIsEmpty", [] {
            LoginPage{}.openPage()
                .typePassword(config::seededPassword)
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::loginRequired);
        }},
        {"shouldShowValidationErrorWhenPasswordIsEmpty", [] {
            LoginPage{}.openPage()
                .typeUsername(config::seededUser)
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::passwordRequired);
        }},
        {"shouldShowErrorWhenPasswordIsWrong", [] {
            LoginPage{}.openPage()
                .typeUsername(config::seededUser)
                .typePassword("wrongpassword")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::wrongCredentials);
        }},
        {"shouldShowValidationErrorWhenUsernameIsTooShort", [] {
            LoginPage{}.openPage()
                .typeUsername("ab")
                .typePassword(config::seededPassword)
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::loginMinLength);
        }},
        {"shouldShowValidationErrorWhenPasswordIsTooShort", [] {
            LoginPage{}.openPage()
                .typeUsername(config::seededUser)
                .typePassword("123")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::passwordMinLength);
        }},
        {"shouldShowErrorWhenUsernameIsUnknown", [] {
            LoginPage{}.openPage()
                .typeUsername("nouser")
                .typePassword(config::seededPassword)
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::wrongCredentials);
        }},
        {"shouldShowValidationErrorWhenCredentialsAreEmpty", [] {
            LoginPage{}.openPage()
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::bothRequired);
        }},
    });
}
