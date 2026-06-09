#include "QuadSolver/QuadSolver.hpp"
#include "BigDecimal/BigDecimal.hpp"
#include <iostream>
#include <string>

int main() {
    std::string input;
    
    while (true) {
        std::cout << "\n1 - Solve equation" << std::endl;
        std::cout << "2 - Exit" << std::endl;
        std::cout << "> ";
        
        std::getline(std::cin, input);
        if (input == "2" || input == "exit") {
            break;
        }
        
        if (input != "1") {
            continue;
        }
        
        std::cout << "Enter a, b, c (space separated): ";
        std::getline(std::cin, input);
        
        size_t firstSpace = input.find(' ');
        size_t secondSpace = input.find(' ', firstSpace + 1);
        
        if (firstSpace == std::string::npos || secondSpace == std::string::npos) {
            std::cout << "WRONG" << std::endl;
            continue;
        }
        
        std::string aStr = input.substr(0, firstSpace);
        std::string bStr = input.substr(firstSpace + 1, secondSpace - firstSpace - 1);
        std::string cStr = input.substr(secondSpace + 1);
        
        BigDecimal a(aStr);
        BigDecimal b(bStr);
        BigDecimal c(cStr);
        
        if (!a.isValid() || !b.isValid() || !c.isValid()) {
            std::cout << "WRONG" << std::endl;
            continue;
        }
        
        Solution sol = QuadSolver::solve(a, b, c);
        printSolution(sol);
    }
    
    return 0;
}