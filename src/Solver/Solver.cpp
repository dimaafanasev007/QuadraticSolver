#include "Solver.hpp"

Result Solver::solve(const BigNumber& a, const BigNumber& b, const BigNumber& c) {
    // Случай a == 0
    if (a.isZero()) {
        return handleLinearCase(b, c);
    }
    
    return handleQuadraticCase(a, b, c);
}

Result Solver::handleLinearCase(const BigNumber& b, const BigNumber& c) {
    // bx + c = 0
    
    if (b.isZero()) {
        // 0*x + c = 0
        if (c.isZero()) {
            return Result::inf();
        } else {
            return Result::noSolution();
        }
    } else {
        // x = -c / b
        BigNumber x = (BigNumber(0) - c) / b;
        return Result::ok({x});
    }
}

Result Solver::handleZeroZeroCase() {
    return Result::inf();
}

Result Solver::handleQuadraticCase(const BigNumber& a, const BigNumber& b, const BigNumber& c) {
    // ax² + bx + c = 0
    
    BigNumber disc = discriminant(a, b, c);
    
    if (disc < BigNumber(0)) {
        return Result::noSolution();
    }
    
    if (disc == BigNumber(0)) {
        // x = -b / (2a)
        BigNumber two = BigNumber(2);
        BigNumber denominator = a * two;
        BigNumber x = (BigNumber(0) - b) / denominator;
        return Result::ok({x});
    }
    
    // Два корня: (-b ± sqrt(disc)) / (2a)
    BigNumber sqrtDisc = disc.sqrt();
    BigNumber two = BigNumber(2);
    BigNumber denominator = a * two;
    
    BigNumber x1 = (BigNumber(0) - b - sqrtDisc) / denominator;
    BigNumber x2 = (BigNumber(0) - b + sqrtDisc) / denominator;
    
    // Сортируем корни (x1 <= x2)
    if (x2 < x1) {
        std::swap(x1, x2);
    }
    
    return Result::ok({x1, x2});
}

BigNumber Solver::discriminant(const BigNumber& a, const BigNumber& b, const BigNumber& c) {
    // D = b² - 4ac
    BigNumber four = BigNumber(4);
    return (b * b) - (four * a * c);
}