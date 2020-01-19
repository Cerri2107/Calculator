#pragma once
#include <vector>
#include <string>

class Calculator {
private:
    std::vector<float>* mem_stack;
public:
    Calculator();
    ~Calculator();
    float memPeek();
    float memPeek(const int& age);
    float doOp(const std::string& op, std::string& left, std::string& right);
    bool isOperator(const std::string& op);
};