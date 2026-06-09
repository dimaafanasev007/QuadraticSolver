#ifndef QUAD_SOLVER_HPP
#define QUAD_SOLVER_HPP

#include "../BigDecimal/BigDecimal.hpp"
#include <string>
#include <vector>

struct Solution {
    bool hasSolutions;
    bool infinite;
    bool wrong;
    std::vector<BigDecimal> realRoots;
    std::vector<std::pair<BigDecimal, BigDecimal>> complexRoots;
    int rootCount;
    std::string errorMessage;
};

class QuadSolver {
public:
    static Solution solve(const BigDecimal& a, const BigDecimal& b, const BigDecimal& c);
};

void printSolution(const Solution& sol);

#endif