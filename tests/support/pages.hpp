// pages.hpp — Page Objects, port of pages/{BasePage,LoginPage,RegisterPage,
// HomePage}.java. Cross-page navigation methods are defined after all classes
// (LoginPage <-> HomePage cyclic refs).
#pragma once

#include "api_client.hpp"

namespace pages {

// --- pages/BasePage.java: backend-scoped auth token helpers -----------------

// AUTH_TOKEN_KEY_JS — the key is resolved from location.pathname so sessions
// are scoped per matrix backend (authToken:backend-java-spring on prod).
inline std::string authTokenKey() {
    return selenide::execute(
               "var m=location.pathname.match(/\\/(backend-[^/]+)\\//);"
               "return m ? 'authToken:' + m[1] : 'authToken';")
        .get<std::string>();
}

inline std::string authToken() {
    json v = selenide::execute("return localStorage.getItem(arguments[0]);",
                               json::array({authTokenKey()}));
    return v.is_null() ? "" : v.get<std::string>();
}

// openPageWithLocalStorageToken: same-origin page first (the logo asset), so
// localStorage is writable under the product mount, then the target route.
inline void openWithToken(const std::string& path, const std::string& token) {
    selenide::open(config::baseUrl + "/icons/qa-guru-logo.svg");
    selenide::execute("localStorage.setItem(arguments[0], arguments[1]);",
                      json::array({authTokenKey(), token}));
    selenide::open(config::baseUrl + path);
}

inline void openWithAuth(const std::string& path, const std::string& username,
                         const std::string& password) {
    openWithToken(path, api::login(username, password));
}

inline void waitAuthTokenCleared() {
    selenide::retry([] { return authToken().empty(); }, "auth token cleared");
}
inline void waitAuthTokenPresent() {
    selenide::retry([] { return !authToken().empty(); }, "auth token present");
}

// --- pages/LoginPage.java ---------------------------------------------------

class HomePage;

class LoginPage {
    selenide::El loginForm    = selenide::$("[data-testid=login-form]");
    selenide::El loginInput   = selenide::$("[data-testid=login-input]");
    selenide::El passwordInput= selenide::$("[data-testid=password-input]");
    selenide::El submitButton = selenide::$("[data-testid=submit-button]");
    selenide::El formTitle    = selenide::$("[data-testid=login-form-title]");
    selenide::El errorMessage = selenide::$("[data-testid=error-message]");

public:
    LoginPage& openPage() {
        selenide::open(config::baseUrl + "/login");
        return shouldBeOpen();
    }
    LoginPage& shouldBeOpen() {
        loginForm.shouldBe(selenide::be::visible);
        return *this;
    }
    LoginPage& typeUsername(const std::string& u) {
        loginInput.setValue(u);
        return *this;
    }
    LoginPage& typePassword(const std::string& p) {
        passwordInput.setValue(p);
        return *this;
    }
    HomePage submit();
    HomePage fillAndSubmitForm(const std::string& u, const std::string& p);
    LoginPage& submitExpectingError() {
        submitButton.click();
        errorMessage.shouldBe(selenide::be::visible);
        return *this;
    }
    LoginPage& shouldHaveFormTitle(const std::string& t) {
        formTitle.shouldHave(selenide::have::text(t));
        return *this;
    }
    LoginPage& shouldHaveErrorMessage(const std::string& m) {
        errorMessage.shouldHave(selenide::have::text(m));
        return *this;
    }
};

// --- pages/RegisterPage.java ------------------------------------------------

class RegisterPage {
    selenide::El registerForm  = selenide::$("[data-testid=register-form]");
    selenide::El loginInput    = selenide::$("[data-testid=register-login-input]");
    selenide::El passwordInput = selenide::$("[data-testid=register-password-input]");
    selenide::El confirmInput  = selenide::$("[data-testid=confirm-password-input]");
    selenide::El submitButton  = selenide::$("[data-testid=register-submit-button]");
    selenide::El formTitle     = selenide::$("[data-testid=register-form-title]");
    selenide::El errorMessage  = selenide::$("[data-testid=register-error-message]");

public:
    RegisterPage& openPage() {
        selenide::open(config::baseUrl + "/register");
        return shouldBeOpen();
    }
    RegisterPage& shouldBeOpen() {
        registerForm.shouldBe(selenide::be::visible);
        return *this;
    }
    RegisterPage& typeUsername(const std::string& u) {
        loginInput.setValue(u);
        return *this;
    }
    RegisterPage& typePassword(const std::string& p) {
        passwordInput.setValue(p);
        return *this;
    }
    RegisterPage& typeConfirmPassword(const std::string& p) {
        confirmInput.setValue(p);
        return *this;
    }
    HomePage submit();
    HomePage fillAndSubmitForm(const std::string& u, const std::string& p,
                               const std::string& confirm);
    RegisterPage& submitExpectingError() {
        submitButton.click();
        errorMessage.shouldBe(selenide::be::visible);
        return *this;
    }
    RegisterPage& shouldHaveErrorMessage(const std::string& m) {
        errorMessage.shouldBe(selenide::be::visible);
        errorMessage.shouldHave(selenide::have::text(m));
        return *this;
    }
};

// --- pages/HomePage.java ----------------------------------------------------

class HomePage {
    selenide::El layout       = selenide::$("[data-testid=multistack-layout]");
    selenide::El healthStatus = selenide::$("[data-testid=health-status]");
    selenide::El itemsList    = selenide::$("[data-testid=items-list]");
    selenide::El welcomePanel = selenide::$("[data-testid=welcome-panel]");
    selenide::El welcomeMessage = selenide::$("[data-testid=welcome-message]");
    selenide::El logoutButton = selenide::$("[data-testid=logout-button]");
    selenide::El deleteButton = selenide::$("[data-testid=delete-account-button]");

public:
    HomePage& openPage() {
        selenide::open(config::baseUrl + "/");
        return shouldBeOpen();
    }
    HomePage& openPageWithLocalStorageAuthentication(const std::string& u,
                                                     const std::string& p) {
        openWithAuth("/", u, p);
        return shouldBeOpen();
    }
    HomePage& openPageWithInvalidToken() {
        openWithToken("/", "invalid-token");
        return shouldBeOpen();
    }
    HomePage& shouldBeOpen() {
        layout.shouldBe(selenide::be::visible);
        return *this;
    }
    HomePage& reloadPage() {
        selenide::execute("location.reload()");
        return shouldBeOpen();
    }
    HomePage& shouldShowHealthText(const std::string& t) {
        healthStatus.shouldHave(selenide::have::text(t));
        return *this;
    }
    HomePage& shouldShowItemText(const std::string& t) {
        itemsList.shouldHave(selenide::have::text(t));
        return *this;
    }
    HomePage& shouldHaveWelcomeMessage(const std::string& m) {
        welcomePanel.shouldBe(selenide::be::visible);
        welcomeMessage.shouldHave(selenide::have::text(m));
        return *this;
    }
    // Panel hides via the HTML hidden attribute when there is no session.
    HomePage& shouldHideWelcomePanel() {
        welcomePanel.shouldHave(selenide::have::attr("hidden", "true"));
        return *this;
    }
    HomePage& shouldShowSessionActions() {
        logoutButton.shouldBe(selenide::be::visible)
            .shouldHave(selenide::have::text("Logout"));
        deleteButton.shouldBe(selenide::be::visible)
            .shouldHave(selenide::have::text("Delete account"));
        return *this;
    }
    LoginPage clickLogoutButton();
    LoginPage clickDeleteAccountAndConfirm();
    HomePage& clickDeleteAccountAndCancel();
    HomePage& shouldClearAuthToken() {
        waitAuthTokenCleared();
        return *this;
    }
    HomePage& shouldKeepAuthToken() {
        waitAuthTokenPresent();
        return *this;
    }
};

// --- cross-page navigation --------------------------------------------------

inline HomePage LoginPage::submit() {
    submitButton.click();
    return HomePage{};
}
inline HomePage LoginPage::fillAndSubmitForm(const std::string& u,
                                             const std::string& p) {
    return typeUsername(u).typePassword(p).submit();
}
inline HomePage RegisterPage::submit() {
    submitButton.click();
    return HomePage{};
}
inline HomePage RegisterPage::fillAndSubmitForm(const std::string& u,
                                                const std::string& p,
                                                const std::string& c) {
    return typeUsername(u).typePassword(p).typeConfirmPassword(c).submit();
}
inline LoginPage HomePage::clickLogoutButton() {
    logoutButton.shouldBe(selenide::be::visible).click();
    return LoginPage{}.shouldBeOpen();
}
// Selenide.confirm(text): assert the dialog copy, then accept it.
inline LoginPage HomePage::clickDeleteAccountAndConfirm() {
    deleteButton.shouldBe(selenide::be::visible).click();
    const std::string actual = selenide::alertText();
    if (actual != msg::deleteConfirm)
        throw std::runtime_error("unexpected confirm text: " + actual);
    selenide::acceptAlert();
    return LoginPage{};
}
inline HomePage& HomePage::clickDeleteAccountAndCancel() {
    deleteButton.shouldBe(selenide::be::visible).click();
    const std::string actual = selenide::alertText();
    if (actual != msg::deleteConfirm)
        throw std::runtime_error("unexpected confirm text: " + actual);
    selenide::dismissAlert();
    return *this;
}

} // namespace pages
