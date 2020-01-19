//don't use these
#include <iostream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

//old
class Calculator {
    vector<float>* mem_stack;

public:
    Calculator() {
        mem_stack = new vector<float>();
    }

    ~Calculator() {
        delete mem_stack;
    }

    float memPeek() {
        return (*mem_stack).back();
    }

    float memPeek(const int& age) {
        return (*mem_stack)[(*mem_stack).size() - age];
    }

    float doOp(const string& op, string& left, string& right) {
        float a, b;

        if (left[0] == 'r') {
            left = left.substr(1, left.size() - 1);
            a = memPeek(stoi(left));
        }
        else
            a = stoi(left);

        if (right[0] == 'r') {
            right = right.substr(1, right.size() - 1);
            b = memPeek(stoi(right));
        }
        else
            b = stoi(right);

        float out;
        if (op == "+")
            out = a + b;
        else if (op == "-")
            out = a - b;
        else if (op == "*")
            out = a * b;
        else if (op == "/")
            out = a / b;
        (*mem_stack).push_back(out);

        return memPeek();
    }
    
    bool isOperator(const string& op) {
        bool out;
        out = op == "+" && op == "-" && op == "*" && op == "/";
        return out;
    }
};