get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(LIB_PATH "." ABSOLUTE)

set(LIB_SOURCES
        ${LIB_PATH}/context.hpp
        ${LIB_PATH}/context.cpp
        ${LIB_PATH}/exceptions.hpp
        ${LIB_PATH}/exceptions.cpp
        ${LIB_PATH}/machine_context.hpp
        ${LIB_PATH}/stack.hpp
        ${LIB_PATH}/trampoline.hpp
        ${LIB_PATH}/stack/mmap.hpp
        ${LIB_PATH}/stack/mmap.cpp
        ${LIB_PATH}/stack/thread.hpp
        ${LIB_PATH}/stack/thread.cpp)

# Architecture-dependent sources

set(LIB_ARCH_SOURCES
        ${LIB_PATH}/arch/${SURE_ARCH}/machine_context.hpp
        ${LIB_PATH}/arch/${SURE_ARCH}/machine_context.cpp
        ${LIB_PATH}/arch/${SURE_ARCH}/machine_context.S)

enable_language(ASM)

add_library(sure STATIC ${LIB_SOURCES} ${LIB_ARCH_SOURCES})
target_include_directories(sure PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(sure PUBLIC wheels pthread)

# Architecture compile definitions

if (SURE_EXCEPTIONS)
    ProjectLog("Capture exceptions context")
    target_compile_definitions(sure PUBLIC SURE_CAPTURE_EXCEPTIONS_CONTEXT)
endif()

target_compile_definitions(sure PUBLIC SURE_ARCH_${SURE_ARCH})
