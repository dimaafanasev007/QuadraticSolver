#ifndef BIGNUMBER_HPP
#define BIGNUMBER_HPP

#include <string>
#include <vector>
#include <iostream>

class BigNumber {
public:
    BigNumber() = default;
    BigNumber(const std::string& str);
    BigNumber(int value);
    
    bool isValid() const { return valid; }
    std::string toString() const;
    
    // Арифметические операции
    BigNumber operator+(const BigNumber& other) const;
    BigNumber operator-(const BigNumber& other) const;
    BigNumber operator*(const BigNumber& other) const;
    BigNumber operator/(const BigNumber& other) const;
    BigNumber operator-() const;
    
    // Сравнение
    bool operator==(const BigNumber& other) const;
    bool operator!=(const BigNumber& other) const;
    bool operator<(const BigNumber& other) const;
    bool operator>(const BigNumber& other) const;
    bool operator<=(const BigNumber& other) const;
    bool operator>=(const BigNumber& other) const;
    
    // Квадратный корень (целочисленный)
    BigNumber sqrt() const;
    
    // Проверка на ноль
    bool isZero() const;
    
    // Получение знака
    int sign() const { return negative ? -1 : (digits.empty() ? 0 : 1); }
    
private:
    std::vector<int> digits;  // цифры в обратном порядке
    bool negative = false;
    bool valid = true;
    
    void normalize();
    void trim();
    static BigNumber addAbsolute(const BigNumber& a, const BigNumber& b);
    static BigNumber subtractAbsolute(const BigNumber& a, const BigNumber& b);
    static int compareAbsolute(const BigNumber& a, const BigNumber& b);
    static BigNumber multiplyByDigit(const BigNumber& a, int digit);
    static BigNumber divideByDigit(const BigNumber& a, int digit, int& remainder);
};

#endif