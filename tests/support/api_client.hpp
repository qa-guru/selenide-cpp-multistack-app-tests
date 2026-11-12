// api_client.hpp — product auth API for setup/cleanup.
// Port of api/AuthApiClient.java: tests arrange throwaway users through
// /auth/* endpoints instead of driving the register form for test data.
#pragma once

#include <curl/curl.h>
#include "test_base.hpp"

namespace api {

namespace detail {
inline size_t onWrite(char* p, size_t s, size_t n, std::string* out) {
    out->append(p, s * n);
    return s * n;
}
} // namespace detail

inline json call(const std::string& method, const std::string& path,
                 const json& body = nullptr, const std::string& bearer = "") {
    CURL* c = curl_easy_init();
    if (!c) throw std::runtime_error("curl_easy_init failed");
    std::string resp, payload;
    curl_slist* hdr = curl_slist_append(nullptr, "Content-Type: application/json");
    if (!bearer.empty())
        hdr = curl_slist_append(hdr, ("Authorization: Bearer " + bearer).c_str());
    curl_easy_setopt(c, CURLOPT_URL, (config::apiBaseUrl + path).c_str());
    curl_easy_setopt(c, CURLOPT_CUSTOMREQUEST, method.c_str());
    curl_easy_setopt(c, CURLOPT_HTTPHEADER, hdr);
    curl_easy_setopt(c, CURLOPT_WRITEFUNCTION, detail::onWrite);
    curl_easy_setopt(c, CURLOPT_WRITEDATA, &resp);
    if (!body.is_null()) {
        payload = body.dump();
        curl_easy_setopt(c, CURLOPT_POSTFIELDS, payload.c_str());
    }
    CURLcode rc = curl_easy_perform(c);
    long status = 0;
    curl_easy_getinfo(c, CURLINFO_RESPONSE_CODE, &status);
    curl_slist_free_all(hdr);
    curl_easy_cleanup(c);
    if (rc != CURLE_OK)
        throw std::runtime_error(std::string("api: ") + curl_easy_strerror(rc));
    json r = resp.empty() ? json::object() : json::parse(resp, nullptr, false);
    if (status >= 400)
        throw std::runtime_error(
            method + " " + path + " -> HTTP " + std::to_string(status) +
            (r.is_object() && r.contains("message")
                 ? ": " + r["message"].get<std::string>()
                 : ""));
    return r;
}

inline std::string login(const std::string& username, const std::string& password) {
    return call("POST", "/auth/login", {{"username", username}, {"password", password}})
        .at("token")
        .get<std::string>();
}

inline std::string reg(const std::string& username, const std::string& password) {
    return call("POST", "/auth/register",
                {{"username", username}, {"password", password}})
        .value("token", "");
}

inline void deleteAccount(const std::string& token) {
    call("DELETE", "/auth/me", nullptr, token);
}

// @AfterEach cleanup — must not mask the original test failure.
inline void deleteAccountQuietly(const std::string& username,
                                 const std::string& password) noexcept {
    try {
        if (!username.empty()) deleteAccount(login(username, password));
    } catch (...) {
    }
}

} // namespace api

// RAII mirror of @AfterEach deleteAccountQuietly: the user is deleted on scope
// exit unless release() is called (UI delete already removed it).
struct UserCleanup {
    std::string user, pass;
    UserCleanup(std::string u, std::string p) : user(std::move(u)), pass(std::move(p)) {}
    ~UserCleanup() { api::deleteAccountQuietly(user, pass); }
    void release() { user.clear(); }
};
