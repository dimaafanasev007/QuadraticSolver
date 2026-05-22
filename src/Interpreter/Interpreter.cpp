#include "Interpreter.hpp"
#include <sstream>
#include <cctype>

Interpreter::ParseResult Interpreter::parse(const std::string& input) {
    ParseResult result;
    result.valid = true;
    
    // Убираем пробелы в начале и конце
    std::string cleaned = trim(input);
    
    // Разделяем по запятым
    std::vector<std::string> tokens = split(cleaned, ',');
    
    if (tokens.size() != 3) {
        result.valid = false;
        result.error = "WRONG";
        return result;
    }
    
    // Проверяем каждый токен на валидность числа
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string token = trim(tokens[i]);
        if (!isValidNumber(token)) {
            result.valid = false;
            result.error = "WRONG";
            return result;
        }
    }
    
    // Создаём BigNumber
    result.a = BigNumber(trim(tokens[0]));
    result.b = BigNumber(trim(tokens[1]));
    result.c = BigNumber(trim(tokens[2]));
    
    if (!result.a.isValid() || !result.b.isValid() || !result.c.isValid()) {
        result.valid = false;
        result.error = "WRONG";
    }
    
    return result;
}

std::string Interpreter::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

bool Interpreter::isValidNumber(const std::string& token) {
    if (token.empty()) return false;
    
    size_t i = 0;
    
    // Проверяем знак
    if (token[0] == '+' || token[0] == '-') {
        i = 1;
        if (token.size() == 1) return false; // Только знак
    }
    
    // Проверяем, что остальное - цифры
    for (; i < token.size(); ++i) {
        if (!std::isdigit(token[i])) {
            return false;
        }
    }
    
    return true;
}

std::vector<std::string> Interpreter::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}