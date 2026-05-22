#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <vector>
#include "../BigNumber/BigNumber.hpp"

// Грамматика:
// Expression ::= Number Whitespace* ',' Whitespace* Number Whitespace* ',' Whitespace* Number
// Number ::= Sign? Digit+
// Sign ::= '+' | '-'
// Digit ::= '0'..'9'
// Whitespace ::= ' ' | '\t'

class Interpreter {
public:
    struct ParseResult {
        BigNumber a;
        BigNumber b;
        BigNumber c;
        bool valid;
        std::string error;
    };
    
    static ParseResult parse(const std::string& input);
    
private:
    static std::string trim(const std::string& str);
    static bool isValidNumber(const std::string& token);
    static std::vector<std::string> split(const std::string& str, char delimiter);
};

#endif