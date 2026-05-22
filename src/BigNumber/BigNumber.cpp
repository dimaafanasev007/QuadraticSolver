#include "BigNumber.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>

BigNumber::BigNumber(const std::string& str) {
    valid = true;
    negative = false;
    
    size_t start = 0;
    if (str[0] == '-') {
        negative = true;
        start = 1;
    } else if (str[0] == '+') {
        start = 1;
    }
    
    for (size_t i = start; i < str.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(str[i]))) {
            valid = false;
            return;
        }
    }
    
    while (start < str.size() && str[start] == '0') {
        start++;
    }
    
    if (start == str.size()) {
        digits = {0};
        negative = false;
        return;
    }
    
    for (size_t i = str.size(); i > start; --i) {
        digits.push_back(static_cast<int>(str[i - 1] - '0'));
    }
    
    trim();
}

BigNumber::BigNumber(int value) {
    if (value < 0) {
        negative = true;
        value = -value;
    } else {
        negative = false;
    }
    
    if (value == 0) {
        digits = {0};
    } else {
        while (value > 0) {
            digits.push_back(value % 10);
            value /= 10;
        }
    }
}

std::string BigNumber::toString() const {
    if (!valid) return "";
    if (digits.empty()) return "0";
    
    std::string result;
    if (negative && !(digits.size() == 1 && digits[0] == 0)) {
        result += "-";
    }
    
    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        result += std::to_string(*it);
    }
    
    return result;
}

void BigNumber::normalize() {
    trim();
    if (digits.size() == 1 && digits[0] == 0) {
        negative = false;
    }
}

void BigNumber::trim() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

bool BigNumber::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

int BigNumber::compareAbsolute(const BigNumber& a, const BigNumber& b) {
    if (a.digits.size() != b.digits.size()) {
        return a.digits.size() < b.digits.size() ? -1 : 1;
    }
    
    for (size_t i = a.digits.size(); i > 0; --i) {
        if (a.digits[i - 1] != b.digits[i - 1]) {
            return a.digits[i - 1] < b.digits[i - 1] ? -1 : 1;
        }
    }
    return 0;
}

bool BigNumber::operator==(const BigNumber& other) const {
    if (!valid || !other.valid) return false;
    if (negative != other.negative) return false;
    return digits == other.digits;
}

bool BigNumber::operator!=(const BigNumber& other) const {
    return !(*this == other);
}

bool BigNumber::operator<(const BigNumber& other) const {
    if (!valid || !other.valid) return false;
    if (negative != other.negative) return negative;
    if (negative) return compareAbsolute(*this, other) > 0;
    return compareAbsolute(*this, other) < 0;
}

bool BigNumber::operator>(const BigNumber& other) const {
    return other < *this;
}

bool BigNumber::operator<=(const BigNumber& other) const {
    return !(*this > other);
}

bool BigNumber::operator>=(const BigNumber& other) const {
    return !(*this < other);
}

BigNumber BigNumber::addAbsolute(const BigNumber& a, const BigNumber& b) {
    BigNumber result;
    result.negative = false;
    
    size_t maxSize = std::max(a.digits.size(), b.digits.size());
    int carry = 0;
    
    for (size_t i = 0; i < maxSize || carry; ++i) {
        int sum = carry;
        if (i < a.digits.size()) sum += a.digits[i];
        if (i < b.digits.size()) sum += b.digits[i];
        
        result.digits.push_back(sum % 10);
        carry = sum / 10;
    }
    
    result.trim();
    return result;
}

BigNumber BigNumber::subtractAbsolute(const BigNumber& a, const BigNumber& b) {
    BigNumber result;
    result.negative = false;
    
    int borrow = 0;
    for (size_t i = 0; i < a.digits.size(); ++i) {
        int diff = a.digits[i] - borrow;
        if (i < b.digits.size()) diff -= b.digits[i];
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digits.push_back(diff);
    }
    
    result.trim();
    return result;
}

BigNumber BigNumber::operator+(const BigNumber& other) const {
    if (!valid || !other.valid) return BigNumber();
    
    if (negative == other.negative) {
        BigNumber result = addAbsolute(*this, other);
        result.negative = negative;
        return result;
    } else {
        int cmp = compareAbsolute(*this, other);
        if (cmp == 0) return BigNumber(0);
        
        BigNumber result;
        if (cmp > 0) {
            result = subtractAbsolute(*this, other);
            result.negative = negative;
        } else {
            result = subtractAbsolute(other, *this);
            result.negative = other.negative;
        }
        return result;
    }
}

BigNumber BigNumber::operator-(const BigNumber& other) const {
    BigNumber negOther = other;
    negOther.negative = !negOther.negative;
    return *this + negOther;
}

BigNumber BigNumber::operator-() const {
    BigNumber result = *this;
    if (!result.isZero()) {
        result.negative = !result.negative;
    }
    return result;
}

BigNumber BigNumber::multiplyByDigit(const BigNumber& a, int digit) {
    BigNumber result;
    int carry = 0;
    
    for (size_t i = 0; i < a.digits.size() || carry; ++i) {
        long long product = carry;
        if (i < a.digits.size()) product += 1LL * a.digits[i] * digit;
        
        result.digits.push_back(static_cast<int>(product % 10));
        carry = static_cast<int>(product / 10);
    }
    
    result.trim();
    return result;
}

BigNumber BigNumber::operator*(const BigNumber& other) const {
    if (!valid || !other.valid) return BigNumber();
    if (isZero() || other.isZero()) return BigNumber(0);
    
    BigNumber result;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    
    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long product = result.digits[i + j] + carry + 
                                1LL * digits[i] * (j < other.digits.size() ? other.digits[j] : 0);
            result.digits[i + j] = static_cast<int>(product % 10);
            carry = static_cast<int>(product / 10);
        }
    }
    
    result.negative = negative != other.negative;
    result.trim();
    return result;
}

BigNumber BigNumber::divideByDigit(const BigNumber& a, int digit, int& remainder) {
    BigNumber result;
    remainder = 0;
    
    for (size_t i = a.digits.size(); i > 0; --i) {
        long long current = remainder * 10LL + a.digits[i - 1];
        result.digits.insert(result.digits.begin(), static_cast<int>(current / digit));
        remainder = static_cast<int>(current % digit);
    }
    
    result.trim();
    return result;
}

BigNumber BigNumber::operator/(const BigNumber& other) const {
    if (!valid || !other.valid || other.isZero()) return BigNumber();
    if (isZero()) return BigNumber(0);
    
    BigNumber result;
    result.negative = negative != other.negative;
    
    BigNumber absA = *this;
    absA.negative = false;
    BigNumber absB = other;
    absB.negative = false;
    
    if (absA < absB) return BigNumber(0);
    
    result.digits.clear();
    BigNumber current;
    
    for (size_t i = absA.digits.size(); i > 0; --i) {
        current.digits.insert(current.digits.begin(), absA.digits[i - 1]);
        current.trim();
        
        int digit = 0;
        while (current >= absB) {
            current = current - absB;
            digit++;
        }
        result.digits.insert(result.digits.begin(), digit);
    }
    
    result.trim();
    return result;
}

BigNumber BigNumber::sqrt() const {
    if (!valid || negative) return BigNumber();
    if (isZero()) return BigNumber(0);
    
    BigNumber low(1);
    BigNumber high = *this;
    BigNumber result(0);
    
    while (low <= high) {
        BigNumber mid = (low + high) / BigNumber(2);
        BigNumber square = mid * mid;
        
        if (square == *this) {
            return mid;
        } else if (square < *this) {
            result = mid;
            low = mid + BigNumber(1);
        } else {
            high = mid - BigNumber(1);
        }
    }
    
    return result;
}