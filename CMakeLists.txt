cmake_minimum_required(VERSION 3.15)
project(QuadraticSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(QuadraticSolver
    src/main.cpp
    src/UnsignedArith/UnsignedArith.cpp
    src/BigDecimal/BigDecimal.cpp
    src/QuadSolver/QuadSolver.cpp
)

add_executable(solver_tests
    tests/test_solver.cpp
    src/UnsignedArith/UnsignedArith.cpp
    src/BigDecimal/BigDecimal.cpp
    src/QuadSolver/QuadSolver.cpp
)

target_link_libraries(solver_tests PRIVATE gtest_main gtest)

target_include_directories(QuadraticSolver PRIVATE src)
target_include_directories(solver_tests PRIVATE src)