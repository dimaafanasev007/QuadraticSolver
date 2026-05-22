#include "Interpreter/Interpreter.hpp"
#include "Solver/Solver.hpp"
#include "Result/Result.hpp"
#include <iostream>
#include <string>

int main() {
    std::string input;
    
    while (true) {
        std::cout << "Enter a,b,c (or 'exit'): ";
        std::getline(std::cin, input);
        
        if (input == "exit" || input == "q") {
            break;
        }
        
        if (input.empty()) {
            continue;
        }
        
        auto parseResult = Interpreter::parse(input);
        
        if (!parseResult.valid) {
            std::cout << "WRONG" << std::endl;
            continue;
        }
        
        auto result = Solver::solve(parseResult.a, parseResult.b, parseResult.c);
        result.print();
        std::cout << std::endl;
    }
    
    return 0;
}