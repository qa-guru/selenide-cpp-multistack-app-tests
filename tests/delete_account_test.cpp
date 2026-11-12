// delete_account_test.cpp — port of tests/e2e/DeleteAccountTests.java.
// Throwaway accounts only — never the seeded user.
#include "support/pages.hpp"

int main() {
    using pages::HomePage;
    using pages::LoginPage;
    return suite::run("DeleteAccountTests", {
        {"confirmingDeleteClearsSessionAndNavigatesToLogin", [] {
            const std::string username = fake::username();
            const std::string password = "password123";
            UserCleanup cleanup{username, password};
            api::reg(username, password);

            HomePage{}.openPageWithLocalStorageAuthentication(username, password)
                .shouldHaveWelcomeMessage("Welcome, " + username + "!")
                .shouldShowSessionActions()
                .clickDeleteAccountAndConfirm()
                .shouldHaveFormTitle("Login Form");

            HomePage{}.shouldClearAuthToken();
            cleanup.release();  // the UI already deleted the account

            // server-side proof: the deleted user can no longer log in
            LoginPage{}.typeUsername(username)
                .typePassword(password)
                .submitExpectingError()
                .shouldHaveErrorMessage(msg::wrongCredentials);
        }},
        {"cancellingConfirmKeepsSession", [] {
            const std::string username = fake::username();
            const std::string password = "password123";
            UserCleanup cleanup{username, password};
            api::reg(username, password);

            HomePage{}.openPageWithLocalStorageAuthentication(username, password)
                .shouldHaveWelcomeMessage("Welcome, " + username + "!")
                .clickDeleteAccountAndCancel()
                .shouldHaveWelcomeMessage("Welcome, " + username + "!")
                .shouldKeepAuthToken();

            api::login(username, password);  // account still alive
        }},
    });
}
