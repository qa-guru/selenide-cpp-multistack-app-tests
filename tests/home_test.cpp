// home_test.cpp — port of tests/e2e/HomeTests.java (smoke).
#include "support/pages.hpp"

int main() {
    using pages::HomePage;
    return suite::run("HomeTests", {
        {"pageLoadFetchesItems", [] {
            HomePage{}.openPage()
                .shouldShowHealthText("service: " + config::healthService)
                .shouldShowItemText("Alpha");
        }},
    });
}
