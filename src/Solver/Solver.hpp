#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "../BigNumber/BigNumber.hpp"
#include "../Result/Result.hpp"
#include <vector>

class Solver {
public:
    static Result solve(const BigNumber& a, const BigNumber& b, const BigNumber& c);
    
private:
    static Result handleLinearCase(const BigNumber& b, const BigNumber& c);
    static Result handleZeroZeroCase();
    static Result handleQuadraticCase(const BigNumber& a, const BigNumber& b, const BigNumber& c);
    static BigNumber discriminant(const BigNumber& a, const BigNumber& b, const BigNumber& c);
    static std::vector<BigNumber> findRoots(const BigNumber& a, const BigNumber& b, const BigNumber& c, const BigNumber& disc, bool hasRealRoots);
};

#endif