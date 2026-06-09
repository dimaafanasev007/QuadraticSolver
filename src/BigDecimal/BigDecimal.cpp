#include "BigDecimal.hpp"
#include "../UnsignedArith/UnsignedArith.hpp"
#include <cctype>
#include <algorithm>

BigDecimal::BigDecimal() : mantissa("0"), exponent(0), negative(false), valid(true) {}

BigDecimal::BigDecimal(long long value) : valid(true) {
    negative = (value < 0);
    std::string num = std::to_string(std::abs(value));
    mantissa = UnsignedArith::trim(num);
    exponent = 0;
}

BigDecimal::BigDecimal(const std::string& str) : valid(true), negative(false), exponent(0) {
    if (str.empty()) { valid = false; return; }
    
    std::string s = str;
    if (s[0] == '-') { negative = true; s = s.substr(1); }
    else if (s[0] == '+') { s = s.substr(1); }
    
    if (s.empty()) { valid = false; return; }
    
    for (char c : s) {
        if (c != '.' && c != 'e' && c != 'E' && c != '+' && c != '-' && !isdigit(c)) {
            valid = false;
            return;
        }
    }
    
    size_t epos = s.find_first_of("eE");
    if (epos != std::string::npos) {
        std::string mant = s.substr(0, epos);
        std::string expStr = s.substr(epos + 1);
        long long exp = 0;
        bool expNeg = false;
        if (!expStr.empty() && expStr[0] == '-') { expNeg = true; expStr = expStr.substr(1); }
        else if (!expStr.empty() && expStr[0] == '+') { expStr = expStr.substr(1); }
        for (char c : expStr) {
            if (!isdigit(c)) { valid = false; return; }
            exp = exp * 10 + (c - '0');
        }
        if (expNeg) exp = -exp;
        *this = BigDecimal(mant, exp, negative);
        return;
    }
    
    size_t dotpos = s.find('.');
    if (dotpos != std::string::npos) {
        std::string intPart = s.substr(0, dotpos);
        std::string fracPart = s.substr(dotpos + 1);
        while (!fracPart.empty() && fracPart.back() == '0') fracPart.pop_back();
        
        if ((intPart.empty() || UnsignedArith::trim(intPart) == "0") && fracPart.empty()) {
            mantissa = "0"; exponent = 0; negative = false;
        } else if (intPart.empty() || UnsignedArith::trim(intPart) == "0") {
            mantissa = fracPart;
            exponent = -(long long)fracPart.size();
        } else if (fracPart.empty()) {
            mantissa = UnsignedArith::trim(intPart);
            exponent = 0;
        } else {
            mantissa = UnsignedArith::trim(intPart) + fracPart;
            exponent = -(long long)fracPart.size();
        }
    } else {
        mantissa = UnsignedArith::trim(s);
        exponent = 0;
    }
    
    if (mantissa == "0" || mantissa.empty()) {
        mantissa = "0";
        negative = false;
        exponent = 0;
    }
}

BigDecimal::BigDecimal(const std::string& digits, long long exp, bool neg)
    : mantissa(UnsignedArith::trim(digits)), exponent(exp), negative(neg), valid(true) {
    if (mantissa == "0") { negative = false; exponent = 0; }
}

bool BigDecimal::isZero() const { return mantissa == "0"; }
int BigDecimal::sign() const { return isZero() ? 0 : (negative ? -1 : 1); }

std::string BigDecimal::toString() const {
    if (!valid || isZero()) return "0";
    std::string result;
    if (negative) result += "-";
    
    if (exponent >= 0) {
        result += mantissa + std::string(exponent, '0');
    } else {
        int intLen = mantissa.length() + exponent;
        if (intLen > 0) {
            result += mantissa.substr(0, intLen) + "." + mantissa.substr(intLen);
        } else {
            result += "0." + std::string(-intLen, '0') + mantissa;
        }
    }
    
    size_t dot = result.find('.');
    if (dot != std::string::npos) {
        while (result.back() == '0') result.pop_back();
        if (result.back() == '.') result.pop_back();
    }
    return result;
}

std::string BigDecimal::toScientific() const {
    if (!valid || isZero()) return "0";
    std::string result;
    if (negative) result += "-";
    
    std::string trimmed = mantissa;
    while (trimmed.length() > 1 && trimmed.back() == '0') trimmed.pop_back();
    result += trimmed[0];
    if (trimmed.length() > 1) result += "." + trimmed.substr(1);
    long long exp = exponent + (long long)trimmed.length() - 1;
    result += "e" + std::to_string(exp);
    return result;
}

std::pair<BigDecimal, BigDecimal> BigDecimal::align(const BigDecimal& a, const BigDecimal& b) {
    BigDecimal a2 = a;
    BigDecimal b2 = b;
    
    // Приводим к одинаковому порядку, добавляя нули в конец мантиссы (умножение на 10^diff)
    if (a2.exponent < b2.exponent) {
        long long diff = b2.exponent - a2.exponent;
        a2.mantissa = a2.mantissa + std::string(diff, '0');
        a2.exponent = b2.exponent;
    } else if (a2.exponent > b2.exponent) {
        long long diff = a2.exponent - b2.exponent;
        b2.mantissa = b2.mantissa + std::string(diff, '0');
        b2.exponent = a2.exponent;
    }
    return {a2, b2};
}

BigDecimal BigDecimal::operator+(const BigDecimal& other) const {
    if (!valid || !other.valid) return BigDecimal();
    if (isZero()) return other;
    if (other.isZero()) return *this;
    
    auto [a, b] = align(*this, other);
    BigDecimal result;
    
    if (negative == other.negative) {
        result.mantissa = UnsignedArith::add(a.mantissa, b.mantissa);
        result.exponent = a.exponent;
        result.negative = negative;
        return result;
    }
    
    int cmp = UnsignedArith::compare(a.mantissa, b.mantissa);
    if (cmp == 0) return BigDecimal("0", 0, false);
    
    if (cmp > 0) {
        result.mantissa = UnsignedArith::subtract(a.mantissa, b.mantissa);
        result.exponent = a.exponent;
        result.negative = negative;
    } else {
        result.mantissa = UnsignedArith::subtract(b.mantissa, a.mantissa);
        result.exponent = a.exponent;
        result.negative = other.negative;
    }
    return result;
}

BigDecimal BigDecimal::operator-(const BigDecimal& other) const {
    BigDecimal neg = other;
    neg.negative = !neg.negative;
    return *this + neg;
}

BigDecimal BigDecimal::operator*(const BigDecimal& other) const {
    if (!valid || !other.valid) return BigDecimal();
    if (isZero() || other.isZero()) return BigDecimal("0", 0, false);
    BigDecimal result;
    result.mantissa = UnsignedArith::multiply(mantissa, other.mantissa);
    result.exponent = exponent + other.exponent;
    result.negative = negative != other.negative;
    return result;
}

BigDecimal BigDecimal::operator/(const BigDecimal& other) const {
    if (!valid || !other.valid || other.isZero()) return BigDecimal();
    if (isZero()) return BigDecimal("0", 0, false);
    auto [a, b] = align(*this, other);
    std::string quot = UnsignedArith::divideWithPrecision(a.mantissa, b.mantissa, 65);
    BigDecimal result;
    result.mantissa = quot;
    result.exponent = a.exponent - b.exponent;
    result.negative = negative != other.negative;
    return result;
}

BigDecimal BigDecimal::operator-() const {
    BigDecimal res = *this;
    if (!res.isZero()) res.negative = !res.negative;
    return res;
}

bool BigDecimal::operator==(const BigDecimal& other) const {
    if (sign() != other.sign()) return false;
    auto [a, b] = align(*this, other);
    return a.mantissa == b.mantissa;
}

bool BigDecimal::operator!=(const BigDecimal& other) const { return !(*this == other); }

bool BigDecimal::operator<(const BigDecimal& other) const {
    if (sign() != other.sign()) return sign() < other.sign();
    auto [a, b] = align(*this, other);
    if (negative) return UnsignedArith::compare(a.mantissa, b.mantissa) > 0;
    return UnsignedArith::compare(a.mantissa, b.mantissa) < 0;
}

bool BigDecimal::operator>(const BigDecimal& other) const { return other < *this; }
bool BigDecimal::operator<=(const BigDecimal& other) const { return !(*this > other); }
bool BigDecimal::operator>=(const BigDecimal& other) const { return !(*this < other); }

BigDecimal BigDecimal::sqrt(int precision) const {
    if (!valid || negative) return BigDecimal();
    if (isZero()) return BigDecimal("0", 0, false);
    
    BigDecimal x = BigDecimal("1");
    BigDecimal two(2);
    
    for (int i = 0; i < precision; ++i) {
        BigDecimal nx = (x + (*this / x)) / two;
        if (x == nx) break;
        x = nx;
        
        std::string xs = x.toString();
        if (xs.length() > precision + 20) {
            xs = xs.substr(0, precision + 20);
            x = BigDecimal(xs);
        }
    }
    return x;
}