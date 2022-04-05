message(STATUS "Context benchmark")

add_executable(context_bench main.cpp)
target_link_libraries(context_bench context wheels)
