cmake_minimum_required(VERSION 3.20)
project(selenide_cpp_multistack_app_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Framework dependency — pinned to the released tag, not a branch.
include(FetchContent)
FetchContent_Declare(selenide
    GIT_REPOSITORY https://github.com/qa-guru/selenide-cpp.git
    GIT_TAG v0.1.1)
FetchContent_MakeAvailable(selenide)

enable_testing()

# Every tests/*_test.cpp is a suite binary (one per canonical Java test class).
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.cpp")
foreach(src ${TEST_SOURCES})
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_link_libraries(${name} PRIVATE selenide::selenide)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${name} PRIVATE -Wall -Wextra)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${name} PRIVATE -Wno-dollar-in-identifier-extension)
    endif()
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES LABELS live TIMEOUT 300)
endforeach()
