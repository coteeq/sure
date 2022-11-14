ProjectLog("Tests")

# List sources

file(GLOB TEST_SOURCES "./*.cpp")

# All tests target

add_executable(rails_tests ${TEST_SOURCES})
target_link_libraries(rails_tests rails wheels)
