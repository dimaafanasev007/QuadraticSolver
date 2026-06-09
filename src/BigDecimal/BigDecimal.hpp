#ifndef BIG_DECIMAL_HPP
#define BIG_DECIMAL_HPP

#include <string>

class BigDecimal {
public:
    BigDecimal();
    BigDecimal(const std::string& str);
    BigDecimal(long long value);
    BigDecimal(const std::string& digits, long long exponent, bool negative);
    
    bool isValid() const { return valid; }
    bool isZero() const;
    int sign() const;
    std::string toString() const;
    std::string toScientific() const;
    
    BigDecimal operator+(const BigDecimal& other) const;
    BigDecimal operator-(const BigDecimal& other) const;
    BigDecimal operator*(const BigDecimal& other) const;
    BigDecimal operator/(const BigDecimal& other) const;
    BigDecimal operator-() const;
    
    bool operator==(const BigDecimal& other) const;
    bool operator!=(const BigDecimal& other) const;
    bool operator<(const BigDecimal& other) const;
    bool operator>(const BigDecimal& other) const;
    bool operator<=(const BigDecimal& other) const;
    bool operator>=(const BigDecimal& other) const;
    
    BigDecimal sqrt(int precision = 65) const;
    
private:
    std::string mantissa;
    long long exponent;
    bool negative;
    bool valid;
    
    void normalize();
    static std::pair<BigDecimal, BigDecimal> align(const BigDecimal& a, const BigDecimal& b);
};

#endif