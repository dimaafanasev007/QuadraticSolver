#include <gtest/gtest.h>
#include "../src/BigNumber/BigNumber.hpp"
#include "../src/Solver/Solver.hpp"
#include "../src/Interpreter/Interpreter.hpp"

// Тесты интерпретатора
TEST(InterpreterTest, ValidInput) {
    auto result = Interpreter::parse("1,2,3");
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.a.toString(), "1");
}

TEST(InterpreterTest, InvalidInputLetters) {
    auto result = Interpreter::parse("a,b,c");
    EXPECT_FALSE(result.valid);
}

TEST(InterpreterTest, ValidWithSpaces) {
    auto result = Interpreter::parse(" 1 , 2 , 3 ");
    EXPECT_TRUE(result.valid);
}

TEST(InterpreterTest, ValidNegative) {
    auto result = Interpreter::parse("-1,-2,-3");
    EXPECT_TRUE(result.valid);
    EXPECT_EQ(result.a.toString(), "-1");
}

// Тесты решения уравнений
TEST(SolverTest, QuadraticTwoRoots) {
    auto result = Solver::solve(BigNumber(1), BigNumber(-3), BigNumber(2));
    EXPECT_EQ(result.getType(), ResultType::OK);
    EXPECT_EQ(result.getRoots().size(), 2);
}

TEST(SolverTest, QuadraticOneRoot) {
    auto result = Solver::solve(BigNumber(1), BigNumber(-2), BigNumber(1));
    EXPECT_EQ(result.getType(), ResultType::OK);
    EXPECT_EQ(result.getRoots().size(), 1);
}

TEST(SolverTest, QuadraticNoRoots) {
    auto result = Solver::solve(BigNumber(1), BigNumber(0), BigNumber(1));
    EXPECT_EQ(result.getType(), ResultType::NO_SOLUTION);
}

TEST(SolverTest, LinearOneRoot) {
    auto result = Solver::solve(BigNumber(0), BigNumber(2), BigNumber(-4));
    EXPECT_EQ(result.getType(), ResultType::OK);
    EXPECT_EQ(result.getRoots().size(), 1);
}

TEST(SolverTest, LinearNoRoots) {
    auto result = Solver::solve(BigNumber(0), BigNumber(0), BigNumber(5));
    EXPECT_EQ(result.getType(), ResultType::NO_SOLUTION);
}

TEST(SolverTest, InfiniteRoots) {
    auto result = Solver::solve(BigNumber(0), BigNumber(0), BigNumber(0));
    EXPECT_EQ(result.getType(), ResultType::INF);
}

// Тесты длинной арифметики
TEST(BigNumberTest, Addition) {
    BigNumber a("123456789");
    BigNumber b("987654321");
    BigNumber c = a + b;
    EXPECT_EQ(c.toString(), "1111111110");
}

TEST(BigNumberTest, Multiplication) {
    BigNumber a("12345");
    BigNumber b("6789");
    BigNumber c = a * b;
    EXPECT_EQ(c.toString(), "83810205");
}

TEST(BigNumberTest, LargeNumbers) {
    std::string large = "123456789012345678901234567890";
    BigNumber bn(large);
    EXPECT_EQ(bn.toString(), large);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}