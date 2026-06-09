#include "UnsignedArith.hpp"
#include <algorithm>
#include <vector>
#include <cctype>

namespace UnsignedArith {

std::string trim(const std::string& s) {
    if (s.empty()) return "0";
    size_t start = 0;
    while (start < s.size() && s[start] == '0') start++;
    if (start == s.size()) return "0";
    return s.substr(start);
}

int compare(const std::string& a, const std::string& b) {
    std::string a2 = trim(a);
    std::string b2 = trim(b);
    if (a2.length() != b2.length()) {
        return a2.length() < b2.length() ? -1 : 1;
    }
    if (a2 < b2) return -1;
    if (a2 > b2) return 1;
    return 0;
}

std::string add(const std::string& a, const std::string& b) {
    std::string result;
    int carry = 0;
    int i = (int)a.size() - 1;
    int j = (int)b.size() - 1;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result.push_back((sum % 10) + '0');
        carry = sum / 10;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string subtract(const std::string& a, const std::string& b) {
    std::string result;
    int borrow = 0;
    int i = (int)a.size() - 1;
    int j = (int)b.size() - 1;
    
    while (i >= 0) {
        int diff = (a[i] - '0') - borrow;
        if (j >= 0) diff -= (b[j] - '0');
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff + '0');
        i--;
        j--;
    }
    std::reverse(result.begin(), result.end());
    return trim(result);
}

std::string multiply(const std::string& a, const std::string& b) {
    if (a == "0" || b == "0") return "0";
    
    std::string a2 = trim(a);
    std::string b2 = trim(b);
    
    int len1 = (int)a2.size();
    int len2 = (int)b2.size();
    std::vector<int> result(len1 + len2, 0);
    
    for (int i = len1 - 1; i >= 0; i--) {
        for (int j = len2 - 1; j >= 0; j--) {
            int mul = (a2[i] - '0') * (b2[j] - '0');
            int sum = mul + result[i + j + 1];
            result[i + j + 1] = sum % 10;
            result[i + j] += sum / 10;
        }
    }
    
    std::string resStr;
    for (int num : result) {
        if (!(resStr.empty() && num == 0)) {
            resStr.push_back(num + '0');
        }
    }
    return resStr.empty() ? "0" : resStr;
}

std::string divide(const std::string& a, const std::string& b, std::string& remainder) {
    if (b == "0") return "0";
    
    std::string dividend = trim(a);
    std::string divisor = trim(b);
    remainder = "0";
    std::string quotient;
    
    for (char ch : dividend) {
        remainder = remainder + ch;
        remainder = trim(remainder);
        
        int count = 0;
        while (compare(remainder, divisor) >= 0) {
            remainder = subtract(remainder, divisor);
            count++;
        }
        quotient.push_back(count + '0');
    }
    
    quotient = trim(quotient);
    if (quotient.empty()) quotient = "0";
    return quotient;
}

std::string divideWithPrecision(const std::string& a, const std::string& b, int precision) {
    if (b == "0") return "0";
    if (a == "0") return "0";
    
    std::string dividend = trim(a);
    std::string divisor = trim(b);
    
    std::string integerPart;
    std::string remainder = "0";
    
    for (char ch : dividend) {
        remainder = remainder + ch;
        remainder = trim(remainder);
        
        int count = 0;
        while (compare(remainder, divisor) >= 0) {
            remainder = subtract(remainder, divisor);
            count++;
        }
        integerPart.push_back(count + '0');
    }
    
    integerPart = trim(integerPart);
    if (integerPart.empty()) integerPart = "0";
    
    std::string result = integerPart;
    
    if (precision > 0 && remainder != "0") {
        result += ".";
        std::string currentRemainder = remainder;
        
        for (int p = 0; p < precision; p++) {
            currentRemainder = currentRemainder + "0";
            currentRemainder = trim(currentRemainder);
            
            int digit = 0;
            while (compare(currentRemainder, divisor) >= 0) {
                currentRemainder = subtract(currentRemainder, divisor);
                digit++;
                if (digit > 10 && p > 40) break;
            }
            result.push_back(digit + '0');
            
            if (currentRemainder == "0") {
                while ((int)result.length() - (int)integerPart.length() - 1 < precision) {
                    result += "0";
                }
                break;
            }
        }
    }
    
    std::string clean;
    for (char c : result) {
        if (c >= '0' && c <= '9') clean += c;
        else if (c == '.') clean += c;
    }
    
    return clean.empty() ? "0" : clean;
}

} // namespace UnsignedArith