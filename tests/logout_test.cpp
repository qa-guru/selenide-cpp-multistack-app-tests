// logout_test.cpp — port of tests/e2e/LogoutTests.java.
#include "support/pages.hpp"

int main() {
    using pages::HomePage;
    using pages::LoginPage;
    return suite::run("LogoutTests", {
        {"shouldLogoutAfterFormLogin", [] {
            LoginPage{}.openPage()
                .fillAndSubmitForm(config::seededUser, config::seededPassword)
                .shouldHaveWelcomeMessage("Welcome, " + config::seededUser + "!")
                .shouldShowSessionActions()
                .clickLogoutButton()
                .shouldHaveFormTitle("Login Form");
        }},
        {"shouldLogoutAfterLocalStorageAuthentication", [] {
            HomePage home = HomePage{}.openPageWithLocalStorageAuthentication(
                                    config::seededUser, config::seededPassword)
                                    .shouldHaveWelcomeMessage(
                                        "Welcome, " + config::seededUser + "!")
                                    .shouldShowSessionActions();
            home.clickLogoutButton().shouldHaveFormTitle("Login Form");
        }},
    });
}
