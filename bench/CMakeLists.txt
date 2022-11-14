ProjectLog("Benchmarks")

add_executable(rails_benchmark main.cpp)
target_link_libraries(rails_benchmark rails wheels)
