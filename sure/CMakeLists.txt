get_filename_component(SURE_INCLUDE_PATH ".." ABSOLUTE)
get_filename_component(SURE_PATH "." ABSOLUTE)

set(SURE_SOURCES
        ${SURE_PATH}/context.hpp
        ${SURE_PATH}/context.cpp
        ${SURE_PATH}/exceptions.hpp
        ${SURE_PATH}/exceptions.cpp
        ${SURE_PATH}/machine/context.hpp
        ${SURE_PATH}/stack.hpp
        ${SURE_PATH}/trampoline.hpp
        ${SURE_PATH}/stack/mmap.hpp
        ${SURE_PATH}/stack/mmap.cpp
        ${SURE_PATH}/stack/thread.hpp
        ${SURE_PATH}/stack/thread.cpp)

# Architecture-dependent sources

ProjectLog("MachineContext: ${SURE_MACHINE_CONTEXT}")

set(SURE_MACHINE_CONTEXT_SOURCES
        ${SURE_PATH}/machine/${SURE_MACHINE_CONTEXT}/context.hpp
        ${SURE_PATH}/machine/${SURE_MACHINE_CONTEXT}/context.cpp
        ${SURE_PATH}/machine/${SURE_MACHINE_CONTEXT}/context.S)

enable_language(ASM)

add_library(sure STATIC ${SURE_SOURCES} ${SURE_MACHINE_CONTEXT_SOURCES})
target_include_directories(sure PUBLIC ${SURE_INCLUDE_PATH})
target_link_libraries(sure PUBLIC wheels pthread)

# Architecture compile definitions

if (SURE_EXCEPTIONS)
    ProjectLog("Capture exceptions context")
    target_compile_definitions(sure PUBLIC SURE_CAPTURE_EXCEPTIONS_CONTEXT)
endif()

target_compile_definitions(sure PUBLIC SURE_MACHINE_CONTEXT_${SURE_MACHINE_CONTEXT})
