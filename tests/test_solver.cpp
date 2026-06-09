#include <gtest/gtest.h>
#include "../src/BigDecimal/BigDecimal.hpp"
#include "../src/QuadSolver/QuadSolver.hpp"

static std::string norm(const std::string& s) {
    std::string r = s;
    size_t dot = r.find('.');
    if (dot != std::string::npos) {
        while (r.back() == '0') r.pop_back();
        if (r.back() == '.') r.pop_back();
    }
    return r;
}

TEST(SolverTest, QuadraticTwoRoots) {
    auto sol = QuadSolver::solve(BigDecimal("1"), BigDecimal("-3"), BigDecimal("2"));
    EXPECT_TRUE(sol.hasSolutions);
    EXPECT_EQ(sol.rootCount, 2);
}

TEST(SolverTest, QuadraticOneRoot) {
    auto sol = QuadSolver::solve(BigDecimal("1"), BigDecimal("-2"), BigDecimal("1"));
    EXPECT_TRUE(sol.hasSolutions);
    EXPECT_EQ(sol.rootCount, 1);
}

TEST(SolverTest, QuadraticComplexRoots) {
    auto sol = QuadSolver::solve(BigDecimal("1"), BigDecimal("0"), BigDecimal("1"));
    EXPECT_TRUE(sol.hasSolutions);
    EXPECT_EQ(sol.rootCount, 2);
    EXPECT_TRUE(sol.realRoots.empty());
}

TEST(SolverTest, LinearOneRoot) {
    auto sol = QuadSolver::solve(BigDecimal("0"), BigDecimal("2"), BigDecimal("-4"));
    EXPECT_TRUE(sol.hasSolutions);
    EXPECT_EQ(sol.rootCount, 1);
    EXPECT_EQ(norm(sol.realRoots[0].toString()), "2");
}

TEST(SolverTest, LinearNoRoots) {
    auto sol = QuadSolver::solve(BigDecimal("0"), BigDecimal("0"), BigDecimal("5"));
    EXPECT_FALSE(sol.hasSolutions);
    EXPECT_FALSE(sol.infinite);
}

TEST(SolverTest, InfiniteRoots) {
    auto sol = QuadSolver::solve(BigDecimal("0"), BigDecimal("0"), BigDecimal("0"));
    EXPECT_TRUE(sol.infinite);
}

TEST(BigDecimalTest, Multiplication) {
    BigDecimal a("123.456"), b("0.5");
    auto res = a * b;
    EXPECT_EQ(norm(res.toString()), "61.728");
}

TEST(BigDecimalTest, Division) {
    BigDecimal a("10"), b("3");
    auto res = a / b;
    std::string s = res.toString();
    EXPECT_TRUE(s.find("3.33") != std::string::npos);
}

TEST(BigDecimalTest, ParseScientific) {
    BigDecimal a("1e-100");
    EXPECT_TRUE(a.isValid());
    EXPECT_FALSE(a.isZero());
}

TEST(BigDecimalTest, LargeAddition) {
    BigDecimal a("999999999999999999999999999999");
    BigDecimal b("1");
    auto res = a + b;
    EXPECT_EQ(res.toString(), "1000000000000000000000000000000");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}