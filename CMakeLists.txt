cmake_minimum_required(VERSION 3.10)

project(LinearSolverBenchmark)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(linear_solver 
    main.cpp 
    solver.cpp 
    solver.h
)

target_compile_options(linear_solver PRIVATE -Wall -Wextra -Wpedantic)