cmake_minimum_required(VERSION 3.5)

message(STATUS "Context tests")

# List sources

file(GLOB TEST_SOURCES "./*.cpp")

# All tests target

add_executable(context_tests ${TEST_SOURCES})
target_link_libraries(context_tests context wheels)
