cmake_minimum_required(VERSION 3.20)
project(selenide_cpp_multistack_app_tests CXX)

include(FetchContent)
FetchContent_Declare(selenide
    GIT_REPOSITORY https://github.com/qa-guru/selenide-cpp.git
    GIT_TAG v0.1.5)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF)
FetchContent_MakeAvailable(selenide googletest)

enable_testing()
include(GoogleTest)

add_executable(login_test tests/login_test.cpp)
target_link_libraries(login_test PRIVATE selenide::selenide GTest::gtest_main)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(login_test PRIVATE -Wall -Wextra)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(login_test PRIVATE -Wno-dollar-in-identifier-extension)
endif()
gtest_discover_tests(login_test PROPERTIES LABELS live TIMEOUT 300)
