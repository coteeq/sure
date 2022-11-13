cmake_minimum_required(VERSION 3.9)
project(context)

include(cmake/Logging.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include(cmake/Sanitize.cmake)

add_subdirectory(third_party)

option(CONTEXT_DEVELOPER "Context development mode" OFF)
option(CONTEXT_EXCEPTIONS "Capture exceptions context" ON)

include(cmake/CompileOptions.cmake)

include(cmake/Platform.cmake)
include(cmake/Arch.cmake)

add_subdirectory(context)

if(CONTEXT_DEVELOPER)
    add_subdirectory(tests)
    add_subdirectory(bench)
endif()
