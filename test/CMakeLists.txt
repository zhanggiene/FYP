cmake_minimum_required(VERSION 3.24)
# These tests can use the Catch2-provided main

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.0 # use catch2 not catch3
)
FetchContent_MakeAvailable(Catch2)


add_executable(cpptest main_approval_test.cpp integration_tests.cpp unit_tests.cpp)
target_link_libraries(cpptest  Catch2::Catch2 eXpressiveDrawing)
add_test(
        NAME cpptest
        COMMAND cpptest)