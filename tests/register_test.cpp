// register_test.cpp — port of tests/e2e/RegisterTests.java.
#include "support/pages.hpp"

int main() {
    using pages::RegisterPage;
    return suite::run("RegisterTests", {
        {"shouldRegisterNewUser", [] {
            // registered through the UI, deleted through the API on scope exit
            UserCleanup user{fake::username(), fake::password()};
            RegisterPage{}.openPage()
                .fillAndSubmitForm(user.user, user.pass, user.pass)
                .shouldHaveWelcomeMessage("Welcome, " + user.user + "!");
        }},
        {"shouldRegisterWithMinimumLengthCredentials", [] {
            UserCleanup user{fake::usernameAtMinLength(), fake::passwordAtMinLength()};
            RegisterPage{}.openPage()
                .fillAndSubmitForm(user.user, user.pass, user.pass)
                .shouldHaveWelcomeMessage("Welcome, " + user.user + "!");
        }},
        {"shouldShowErrorWhenPasswordsDoNotMatch", [] {
            RegisterPage{}.openPage()
                .typeUsername("newuser")
                .typePassword("password123")
                .typeConfirmPassword("password124")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::passwordMismatch);
        }},
        {"shouldShowErrorWhenPasswordIsTooShort", [] {
            RegisterPage{}.openPage()
                .typeUsername("newuser")
                .typePassword("abc")
                .typeConfirmPassword("abc")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::passwordMinLength);
        }},
        {"shouldShowErrorWhenUsernameIsTaken", [] {
            RegisterPage{}.openPage()
                .typeUsername(config::seededUser)
                .typePassword("password123")
                .typeConfirmPassword("password123")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::duplicateUser);
        }},
        {"shouldShowValidationErrorWhenUsernameIsTooShort", [] {
            RegisterPage{}.openPage()
                .typeUsername("ab")
                .typePassword("password123")
                .typeConfirmPassword("password123")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::loginMinLength);
        }},
        {"shouldShowValidationErrorWhenUsernameIsEmpty", [] {
            RegisterPage{}.openPage()
                .typePassword("password123")
                .typeConfirmPassword("password123")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::loginRequired);
        }},
        {"shouldShowValidationErrorWhenPasswordIsEmpty", [] {
            RegisterPage{}.openPage()
                .typeUsername("newuser")
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::passwordRequired);
        }},
        {"shouldShowValidationErrorWhenCredentialsAreEmpty", [] {
            RegisterPage{}.openPage()
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::bothRequired);
        }},
    });
}
