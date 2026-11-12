cmake_minimum_required(VERSION 3.20)
project(selenide_cpp_multistack_app_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Framework dependency — pinned to the released tag, not a branch.
include(FetchContent)
FetchContent_Declare(selenide
    GIT_REPOSITORY https://github.com/qa-guru/selenide-cpp.git
    GIT_TAG v0.1.0)
FetchContent_MakeAvailable(selenide)

enable_testing()

foreach(t multistack new_app)
    add_executable(${t} tests/${t}.cpp)
    target_link_libraries(${t} PRIVATE selenide::selenide)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${t} PRIVATE -Wall -Wextra)
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${t} PRIVATE -Wno-dollar-in-identifier-extension)
    endif()
    add_test(NAME ${t} COMMAND ${t})
    set_tests_properties(${t} PROPERTIES LABELS live TIMEOUT 120)
endforeach()
