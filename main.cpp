#include <iostream>
#include <cmath>
#include <string>
#include <vector>

using namespace std;

class Calculator {

    vector<float>* mem_stack;
	vector<string>* lineArgs;
	int cnt = 0;

    public:
        struct Output {
            float value;
            bool valid;
			Output() {
				value = 0;
				valid = true;
			}
        };

        Calculator() {
            mem_stack = new vector<float>;
			lineArgs = new vector<string>;
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

		Output strToOperand(const string& operand) {
			Output output;
			string& prefix = operand.substr(0, operand.find_first_of('_'));
			string& suffix = operand.substr(operand.find_first_of('_') + 1);
			if (prefix == "r") {
				if (output.valid = isInt(suffix)) {
					int age = stoi(suffix);
					if (output.valid = age < (*mem_stack).size() + 1)
						output.value = memPeek(age);
				}
                return output;
			}
			else if (prefix == "sqrt") {
				Output num = strToOperand(suffix);
				if (output.valid = num.valid)
					output.value = sqrtf(num.value);
				return output;
			}
            else {
				if (output.valid = isFloat(operand))
					output.value = stof(operand);
                return output;
			}
		}

        Output executeOp(const Output& o_left, const string& op, string& right) {
            Output out = strToOperand(right);

			if (!(out.valid = out.valid && isOperator(op) && o_left.valid))
				return out;

            if (op == "+")
                out.value = o_left.value + out.value;
            else if (op == "-")
                out.value = o_left.value - out.value;
            else if (op == "*")
                out.value = o_left.value * out.value;
            else if (op == "/")
                out.value = o_left.value / out.value;

			cnt += 2;
			if (cnt < (*lineArgs).size() - 1)
            	return executeOp(out, (*lineArgs)[cnt + 1], (*lineArgs)[cnt + 2]);
			else
				return out;
        }

        Output lineOp(string& line) {
			Output out;
			(*lineArgs).push_back("");
			string left, op, right;
			int i = 0;

			while(line.length() > 0) {
				if (line[0] == ' ') {
					while(line[0] == ' ')
						line = line.substr(1);
					(*lineArgs).push_back("");
					i++;
				}
				(*lineArgs)[i].push_back(line[0]);
				line = line.substr(1);
			}

			if (out.valid = (*lineArgs).size() > 2 && (*lineArgs).size() % 2 == 1) {
				cnt = 0;
				out = executeOp(strToOperand((*lineArgs)[cnt]), (*lineArgs)[cnt + 1], (*lineArgs)[cnt + 2]);
				if (out.valid)
            		(*mem_stack).push_back(out.value);
			}

			(*lineArgs).erase((*lineArgs).begin(), (*lineArgs).end());
			return out;
        }
        
        bool isOperator(const string& op) {
            bool out;
            out = op == "+" || op == "-" || op == "*" || op == "/";
            return out;
        }
		bool isFloat(const string& s) {
            bool out = true;
			int dots = 0;
			for (int i = 0; i < s.length(); i++)
				if (!isdigit(s[i]) && s[i] != '.')
					out = false;
				else if (s[i] == '.')
					dots++;
            return out && dots < 2;
        }
		bool isInt(const string& s) {
            bool out = true;
			for (int i = 0; i < s.length(); i++)
				if (!isdigit(s[i]))
					out = false;
            return out;
        }
};

int main() {
    Calculator calc;
    Calculator::Output out;
    string input;
    do {
        cout << endl << "insert an operator and two numbers: ";
        getline(cin, input);
        if (input == "exit")
            break;
		if (input != "") {
			out = calc.lineOp(input);
        	if (out.valid)
            	cout << "\t\t\t    result: " << out.value;
        	else
        		cout << "invalid operation";
			cout << endl << "-----------------------------------";
		}

    } while(true);
}


        // string left, right;
        // float out;

        // cout << endl << "insert an operator and two numbers: ";
        // cin >> op;
        // if (op != "exit") {
        //     cin >> left;
        //     cin >> right;
        //     if (calc.isOperator(op)) {
        //         out = calc.doOp(op, left, right);
        //         cout << endl << "result: " << out << endl;
        //     }
        //     else
        //         cout << endl << "invalid operator" << endl;
        // }
        // else 
        //     break;