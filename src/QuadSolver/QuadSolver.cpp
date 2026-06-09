#include "QuadSolver.hpp"
#include <iostream>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <sstream>

static Solution handleLinear(const BigDecimal& b, const BigDecimal& c) {
    Solution sol;
    sol.wrong = false;
    sol.infinite = false;
    sol.hasSolutions = true;
    if (b.isZero()) {
        if (c.isZero()) {
            sol.infinite = true;
            sol.rootCount = -1;
        } else {
            sol.hasSolutions = false;
            sol.rootCount = 0;
        }
        return sol;
    }
    // Линейное уравнение через double для стабильности
    double db = std::stod(b.toString());
    double dc = std::stod(c.toString());
    double x = -dc / db;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(60) << x;
    std::string xs = ss.str();
    while (xs.back() == '0') xs.pop_back();
    if (xs.back() == '.') xs.pop_back();
    sol.realRoots.push_back(BigDecimal(xs));
    sol.rootCount = 1;
    return sol;
}

static Solution handleQuadratic(const BigDecimal& a, const BigDecimal& b, const BigDecimal& c) {
    Solution sol;
    sol.wrong = false;
    sol.infinite = false;
    sol.hasSolutions = true;
    
    // Переводим в double
    double da = std::stod(a.toString());
    double db = std::stod(b.toString());
    double dc = std::stod(c.toString());
    
    double disc = db*db - 4.0*da*dc;
    double twoA = 2.0*da;
    double negB = -db;
    
    auto format = [](double value) -> BigDecimal {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(60) << value;
        std::string s = ss.str();
        // Убираем хвостовые нули
        while (s.back() == '0') s.pop_back();
        if (s.back() == '.') s.pop_back();
        return BigDecimal(s);
    };
    
    if (disc > 1e-15) {
        double sqrtDisc = std::sqrt(disc);
        double x1 = (negB - sqrtDisc) / twoA;
        double x2 = (negB + sqrtDisc) / twoA;
        if (x2 < x1) std::swap(x1, x2);
        sol.realRoots.push_back(format(x1));
        sol.realRoots.push_back(format(x2));
        sol.rootCount = 2;
    } else if (std::abs(disc) < 1e-15) {
        double x = negB / twoA;
        sol.realRoots.push_back(format(x));
        sol.rootCount = 1;
    } else {
        double sqrtDisc = std::sqrt(-disc);
        double real = negB / twoA;
        double imag = sqrtDisc / twoA;
        BigDecimal re = format(real);
        BigDecimal im = format(imag);
        sol.complexRoots.push_back({re, im});
        sol.complexRoots.push_back({re, im * BigDecimal(-1)});
        sol.rootCount = 2;
    }
    return sol;
}

Solution QuadSolver::solve(const BigDecimal& a, const BigDecimal& b, const BigDecimal& c) {
    if (!a.isValid() || !b.isValid() || !c.isValid()) {
        Solution sol;
        sol.wrong = true;
        return sol;
    }
    
    if (a.isZero()) {
        return handleLinear(b, c);
    }
    return handleQuadratic(a, b, c);
}

void printSolution(const Solution& sol) {
    if (sol.wrong) {
        std::cout << "WRONG" << std::endl;
        return;
    }
    if (sol.infinite) {
        std::cout << "INF" << std::endl;
        return;
    }
    if (!sol.hasSolutions) {
        std::cout << "NO SOLUTION" << std::endl;
        return;
    }
    
    std::cout << "OK" << std::endl;
    std::cout << sol.rootCount << std::endl;
    
    for (const auto& root : sol.realRoots) {
        std::cout << root.toString() << std::endl;
    }
    
    for (const auto& [re, im] : sol.complexRoots) {
        std::cout << re.toString();
        if (im.sign() >= 0) std::cout << "+";
        std::cout << im.toString() << "i" << std::endl;
    }
}