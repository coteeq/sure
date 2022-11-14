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
        ${LIB_PATH}/arch/${CONTEXT_ARCH}/machine_context.hpp
        ${LIB_PATH}/arch/${CONTEXT_ARCH}/machine_context.cpp
        ${LIB_PATH}/arch/${CONTEXT_ARCH}/machine_context.S)

enable_language(ASM)

add_library(rails STATIC ${LIB_SOURCES} ${LIB_ARCH_SOURCES})
target_include_directories(rails PUBLIC ${LIB_INCLUDE_PATH})
target_link_libraries(rails PUBLIC wheels pthread)

# Architecture compile definitions

if (RAILS_EXCEPTIONS)
    ProjectLog("Capture exceptions context")
    target_compile_definitions(rails PUBLIC RAILS_CAPTURE_EXCEPTIONS_CONTEXT)
endif()

target_compile_definitions(rails PUBLIC arch_${CONTEXT_ARCH})
