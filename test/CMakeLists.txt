cmake_minimum_required(VERSION 3.24)
find_package(Eigen3 REQUIRED)
# These tests can use the Catch2-provided main

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)
file(GLOB_RECURSE TEST_FILES "*tests.cpp")
add_executable(cpptest main.cpp ${TEST_FILES} ../include/stb_image.h ../include/stb_image_write.h)
target_link_libraries(cpptest Eigen3::Eigen Catch2::Catch2WithMain)
add_custom_target(test
        COMMENT "Running C++ tests..."
        COMMAND $<TARGET_FILE:cpptest>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})