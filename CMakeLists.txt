cmake_minimum_required(VERSION 3.16)

project(hft_data_structures_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O3 -march=native)

include_directories(include)

add_executable(
    benchmark_hash_table
    benchmarks/benchmark_hash_table.cpp
)

add_executable(
    benchmark_priority_queue
    benchmarks/benchmark_priority_queue.cpp
)