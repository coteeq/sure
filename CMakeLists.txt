cmake_minimum_required(VERSION 3.9)
project(rails)

include(cmake/Logging.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include(cmake/Sanitize.cmake)

add_subdirectory(third_party)

option(RAILS_DEVELOPER "Rails development mode" OFF)
option(RAILS_EXCEPTIONS "Rails exceptions context" ON)

include(cmake/CompileOptions.cmake)

include(cmake/Platform.cmake)
include(cmake/Arch.cmake)

add_subdirectory(rails)

if(RAILS_DEVELOPER)
    add_subdirectory(tests)
    add_subdirectory(bench)
endif()
