//don't use these
#pragma once
#include <vector>
#include <string>

class Calculator {
private:
    std::vector<float>* mem_stack;
	std::vector<std::string>* lineArgs;
public:
    struct Output {
        float value;
        bool valid;
        Output();
    };
    Calculator();
    ~Calculator();
    float memPeek(const int& age);
	Output strToOperand(const std::string& operand);
    Output executeOp(const Output& o_left, const std::string& op, std::string& right);
    Output lineOp(const std::string& line);
    bool isOperator(const std::string& op);
	bool isFloat(const std::string& s);
    bool isInt(const std::string& s);
};