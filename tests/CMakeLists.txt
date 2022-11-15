ProjectLog("Tests")

# List sources

file(GLOB TEST_SOURCES "./*.cpp")

# All tests target

add_executable(sure_tests ${TEST_SOURCES})
target_link_libraries(sure_tests sure wheels)
