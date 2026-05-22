#ifndef RESULT_HPP
#define RESULT_HPP

#include <string>
#include <vector>
#include "../BigNumber/BigNumber.hpp"

enum class ResultType {
    OK,
    INF,
    WRONG,
    NO_SOLUTION
};

class Result {
public:
    Result(ResultType type, const std::vector<BigNumber>& roots = {});
    
    static Result ok(const std::vector<BigNumber>& roots);
    static Result inf();
    static Result wrong();
    static Result noSolution();
    
    ResultType getType() const { return type; }
    const std::vector<BigNumber>& getRoots() const { return roots; }
    
    void print() const;
    
private:
    ResultType type;
    std::vector<BigNumber> roots;
};

#endif