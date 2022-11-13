ProjectLog("Context tests")

# List sources

file(GLOB TEST_SOURCES "./*.cpp")

# All tests target

add_executable(context_tests ${TEST_SOURCES})
target_link_libraries(context_tests context wheels)
