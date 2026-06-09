#ifndef UNSIGNED_ARITH_HPP
#define UNSIGNED_ARITH_HPP

#include <string>

namespace UnsignedArith {
    int compare(const std::string& a, const std::string& b);
    std::string trim(const std::string& s);
    std::string add(const std::string& a, const std::string& b);
    std::string subtract(const std::string& a, const std::string& b);
    std::string multiply(const std::string& a, const std::string& b);
    std::string divide(const std::string& a, const std::string& b, std::string& remainder);
    std::string divideWithPrecision(const std::string& a, const std::string& b, int precision);
}

#endif