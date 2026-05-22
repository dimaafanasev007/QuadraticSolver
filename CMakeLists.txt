cmake_minimum_required(VERSION 3.10)
project(QuadraticSolver VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Принудительно используем MT (статический рантайм) везде
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT")
endif()

# Основная программа
add_executable(quadratic_solver
    src/main.cpp
    src/Interpreter/Interpreter.cpp
    src/BigNumber/BigNumber.cpp
    src/Solver/Solver.cpp
    src/Result/Result.cpp
)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Принудительно меняем рантайм у Google Test на MT
if(MSVC)
    target_compile_options(gtest PRIVATE /MT)
    target_compile_options(gtest_main PRIVATE /MT)
endif()

# Тесты с Google Test
add_executable(solver_tests
    tests/test_solver.cpp
    src/BigNumber/BigNumber.cpp
    src/Solver/Solver.cpp
    src/Interpreter/Interpreter.cpp
    src/Result/Result.cpp
)

target_link_libraries(solver_tests PRIVATE gtest_main)

target_include_directories(quadratic_solver PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(solver_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(quadratic_solver PRIVATE /W4)
    target_compile_options(solver_tests PRIVATE /W4)
endif()