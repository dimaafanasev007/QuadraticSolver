#include "Result.hpp"
#include <iostream>

Result::Result(ResultType type, const std::vector<BigNumber>& roots)
    : type(type), roots(roots) {}

Result Result::ok(const std::vector<BigNumber>& roots) {
    return Result(ResultType::OK, roots);
}

Result Result::inf() {
    return Result(ResultType::INF);
}

Result Result::wrong() {
    return Result(ResultType::WRONG);
}

Result Result::noSolution() {
    return Result(ResultType::NO_SOLUTION);
}

void Result::print() const {
    switch (type) {
        case ResultType::OK:
            std::cout << "OK\n";
            std::cout << roots.size() << "\n";
            for (const auto& root : roots) {
                std::cout << root.toString() << "\n";
            }
            break;
        case ResultType::INF:
            std::cout << "INF\n";
            break;
        case ResultType::WRONG:
            std::cout << "WRONG\n";
            break;
        case ResultType::NO_SOLUTION:
            std::cout << "NO SOLUTION\n";
            break;
    }
}