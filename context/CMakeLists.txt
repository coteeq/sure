cmake_minimum_required(VERSION 3.5)

# --------------------------------------------------------------------

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

file(GLOB_RECURSE LIB_ARCH_DEPENDS_ALL ${LIB_PATH}/arch/*.hpp
                                       ${LIB_PATH}/arch/*.cpp
                                       ${LIB_PATH}/arch/*.S)

file(GLOB_RECURSE LIB_SOURCES ${LIB_PATH}/*.hpp
                              ${LIB_PATH}/*.cpp
                              ${LIB_PATH}/*.S)

list(REMOVE_ITEM LIB_SOURCES ${LIB_ARCH_DEPENDS_ALL})

# Architecture-dependent sources

set(LIB_ARCH_DEPENDS ${LIB_PATH}/arch/${CTX_ARCH}/machine_context.hpp
                     ${LIB_PATH}/arch/${CTX_ARCH}/machine_context.cpp
                     ${LIB_PATH}/arch/${CTX_ARCH}/machine_context.S)

enable_language(ASM)

add_library(context STATIC ${LIB_SOURCES} ${LIB_ARCH_DEPENDS})
target_include_directories(context PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(context PUBLIC wheels pthread)

# Architecture compile definitions

target_compile_definitions(context PUBLIC ${CTX_ARCH})
