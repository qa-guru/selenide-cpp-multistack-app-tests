// session_test.cpp — port of tests/e2e/SessionTests.java.
#include "support/pages.hpp"

int main() {
    using pages::HomePage;
    return suite::run("SessionTests", {
        {"invalidTokenClearsSession", [] {
            HomePage{}.openPageWithInvalidToken()
                .shouldHideWelcomePanel()
                .shouldClearAuthToken();
        }},
        {"sessionSurvivesReload", [] {
            HomePage{}.openPageWithLocalStorageAuthentication(
                             config::seededUser, config::seededPassword)
                .shouldHaveWelcomeMessage("Welcome, " + config::seededUser + "!")
                .reloadPage()
                .shouldHaveWelcomeMessage("Welcome, " + config::seededUser + "!");
        }},
    });
}
