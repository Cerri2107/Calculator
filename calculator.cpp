#include <iostream>
#include <cmath>
#include <deque>
#include <algorithm>
#include <string>

namespace cerri {
	//deque wrapper
	template<typename Type = float>
	class deck {
		std::deque<Type> deq;
	public:
		//return memorized result of a set age (how many operations ago it was saved)
		Type peek(const size_t& age) {
			return deq[deq.size() - age];
		}
		void push_back(const Type& value) {
			deq.push_back(value);
		}
		Type pop_front() {
			Type out = deq.front();
			deq.pop_front();
			return out;
		}
		size_t size() {
			return deq.size();
		}
	};

	//calculator class
	class calculator {
	public:
		//can keep the result of any operation, and its validity
		struct out_t {
			float value = 0;
			bool valid = true;
		};

	private:
		deck<float> memory;										//keeps every old result
		std::deque<std::string> operations;						//keeps every part of an operation
		std::deque<std::string> ops_pr_1 { "*", "/", "^", "v" };//operators to execute first
		std::deque<std::string> ops_pr_2 { "+", "-" };			//operators to execute second
		std::deque<std::string>::iterator op_count;				//keeps track of which operator to execute

		//returns iterator reference of next operation to execute
		std::deque<std::string>::iterator nextop() {
			std::deque<std::string>::iterator out;
			out = std::find_first_of(operations.begin(), operations.end(), ops_pr_1.begin(), ops_pr_1.end());
			if (out == operations.end())
				out = std::find_first_of(operations.begin(), operations.end(), ops_pr_2.begin(), ops_pr_2.end());
			return out;
		}
		//recursively converts an "operand" (which includes r_{num}, sqrt_{num}, etc...) to real numbers kept in the struct out_t
		out_t strtoop(const std::string& operand) {
			out_t output;
			std::string prefix = operand.substr(0, operand.find_first_of('_'));
			std::string suffix = operand.substr(operand.find_first_of('_') + 1);

			//checks for every preset prefix

			#pragma region if "r"
			if (prefix == "r") {						//r_{num} returns number of num age from "memory stack"
				if (output.valid = isInt(suffix)) {
					int age = stoi(suffix);
					if (output.valid = age < memory.size() + 1)
						output.value = memory.peek(age);
				}
				return output;
			}
			#pragma endregion

			#pragma region if "sqrt"
			else if (prefix == "sqrt") {				//sqrt_{num} square root of num
				out_t num = strtoop(suffix);
				if (output.valid = num.valid)
					output.value = sqrtf(num.value);
				return output;
			}
			#pragma endregion

			#pragma region if "sqr"
			else if (prefix == "sqr") {					//sqr_{num}	square of num
				out_t num = strtoop(suffix);
				if (output.valid = num.valid)
					output.value = num.value * num.value;
				return output;
			}
			#pragma endregion

			#pragma region if a number
			else {										//returns the suffix as a float number
				if (output.valid = isFloat(suffix))
					output.value = stof(suffix);
				return output;
			}
			#pragma endregion

		}
		//recursively executes the operation
		out_t execop(const out_t& o_left, const std::string& op, std::string& right) {
			out_t out = strtoop(right);

			if (!(out.valid = out.valid && o_left.valid))
				return out;

			#pragma region operations
			if (op == "+")
				out.value = o_left.value + out.value;
			else if (op == "-")
				out.value = o_left.value - out.value;
			else if (op == "*")
				out.value = o_left.value * out.value;
			else if (op == "/")
				out.value = o_left.value / out.value;
			else if (op == "^")
				out.value = powf(o_left.value, out.value);
			else if (op == "v")
				out.value = powf(o_left.value, 1 / out.value);
			else {
				out.valid = false;
				return out;
			}
			#pragma endregion

			operations.erase(op_count, op_count + 2);
			op_count = nextop();
			if (op_count != operations.end())
				return execop(out, *op_count, *(op_count + 1));
			else
				return out;
		}

	public:
		//starts the execution of a line of operations
		out_t startop(std::string& line) {
			out_t out;
			operations.push_back("");
			std::string left, op, right;
			int i = 0;
			while (line.length() > 0) {
				if (line[0] == ' ') {
					while (line[0] == ' ')
						line = line.substr(1);
					operations.push_back("");
					i++;
				}
				operations[i].push_back(line[0]);
				line = line.substr(1);
			}

			if (out.valid = operations.size() > 2 && operations.size() % 2 == 1) {
				op_count = nextop();
				out = execop(strtoop(*(op_count - 1)), *op_count, *(op_count + 1));
				if (out.valid)
					memory.push_back(out.value);
			}

			operations.erase(operations.begin(), operations.end());
			return out;
		}
		//return whether a string contains a valid float number
		bool isFloat(const std::string& s) {
			bool dot = false;
			for (int i = 0; i < s.length(); i++)
				if (!isdigit(s[i]) && s[i] != '.')
					return false;
				else if (s[i] == '.' && !dot)
					dot = true;
				else if (s[i] == '.' && dot)
					return false;
			return true;
		}
		//return whether a string contains a valid integer number
		bool isInt(const std::string& s) {
			for (int i = 0; i < s.length(); i++)
				if (!isdigit(s[i])/* && i != 0 || s[i] != '-' || s[i] != '+'*/)
					return false;
			return true;
		}
	};
}