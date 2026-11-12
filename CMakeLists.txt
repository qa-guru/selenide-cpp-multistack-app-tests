cmake_minimum_required(VERSION 3.20)
project(selenide_cpp_multistack_app_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Framework dependency — pinned to the released tag, not a branch.
include(FetchContent)
FetchContent_Declare(selenide
    GIT_REPOSITORY https://github.com/qa-guru/selenide-cpp.git
    GIT_TAG v0.1.2)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(selenide googletest)

enable_testing()
include(GoogleTest)

# Every tests/*_test.cpp is a suite binary; each TEST_F registers as
# its own CTest case via gtest_discover_tests.
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.cpp")
foreach(src ${TEST_SOURCES})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE selenide::selenide GTest::gtest_main)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${name} PRIVATE -Wall -Wextra)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${name} PRIVATE -Wno-dollar-in-identifier-extension)
    endif()
    gtest_discover_tests(${name} PROPERTIES LABELS live TIMEOUT 300)
endforeach()
