#include <iostream>
#include <cmath>
#include <deque>
#include <algorithm>
#include <string>

namespace cerri {
	//calculator class
	class calculator {
		//can keep the result of any operation, and its validity
		struct out_t {
			float value = 0;
			bool valid = true;
		};

		//defined string deque as a type for brevity
		typedef std::deque<std::string> str_deque;
		std::deque<float> memory;								//keeps every old result
		str_deque tokens;								//keeps every "word" of an expression
		str_deque ops_pr_1 { "^", "v", "log", "ln" };	//operators to execute first
		str_deque ops_pr_2{ "*", "/" };					//operators to execute second
		str_deque ops_pr_3 { "+", "-" };				//operators to execute last
		str_deque::iterator exec_point;					//keeps track of which operator is being executed

		//returns a pair containing begin and end iterators of the last parenthesis divided snippet in the tokens deque*/
		std::pair<str_deque::iterator, str_deque::iterator> getnextsnippet() {
			str_deque::iterator begin, end;
			str_deque::reverse_iterator rbegin;

			rbegin = std::find(tokens.rbegin(), tokens.rend(), "(");
			if (rbegin != tokens.rend())
				rbegin++;
			begin = rbegin.base();
			if (begin == tokens.end())
				begin = tokens.begin();
			end = std::find(begin, tokens.end(), ")");

			return std::pair<str_deque::iterator, str_deque::iterator>(begin, end);
		}
		//returns a string deque containing the tokens of the expression
		str_deque dividetokens(std::string exp) {
			str_deque out;
			out.push_back("");
			int i = 0;
			while (exp.length() > 0) {
				if (exp[0] == ' ') {
					while (exp[0] == ' ')
						exp = exp.substr(1);
					out.push_back("");
					i++;
				}
				out[i].push_back(exp[0]);
				exp = exp.substr(1);
			}
			return out;
		}
		//returns iterator reference of next operation to execute
		str_deque::iterator nextop() {
			str_deque::iterator out;
			std::pair<str_deque::iterator, str_deque::iterator> snip = getnextsnippet();
			str_deque::iterator snip_begin = snip.first;
			str_deque::iterator snip_end = snip.second;

			out = std::find_first_of(snip_begin, snip_end, ops_pr_1.begin(), ops_pr_1.end());
			if (out == snip_end) {
				out = std::find_first_of(snip_begin, snip_end, ops_pr_2.begin(), ops_pr_2.end());
				if (out == snip_end)
					out = std::find_first_of(snip_begin, snip_end, ops_pr_3.begin(), ops_pr_3.end());
			}

			size_t out_i = std::distance(tokens.begin(), out);
			size_t end_i = std::distance(tokens.begin(), snip_end);
			if (snip_end - snip_begin < 5) {
				if (*snip_begin == "(") {
					tokens.erase(snip_begin);
					out_i--;
					end_i--;
				}
				if (tokens.begin() + end_i != tokens.end())
					tokens.erase(tokens.begin() + end_i);
				out = tokens.begin() + out_i;
			}

			return out;
		}
		//recursively converts an "operand" (which includes r_{num}, sqrt_{num}, etc...) to real numbers kept in the struct out_t
		out_t strtoop(const std::string& operand) {
			out_t output;
			std::string prefix = operand.substr(0, operand.find_first_of('('));
			std::string suffix = operand.substr(prefix.length(), operand.find_last_of(')'));

			//checks the prefix for all the preset ones

			#pragma region if "r"
			if (prefix == "r") {						//r_{num} returns number of num age from memory
				if (output.valid = isunsint(suffix)) {
					size_t age = stoi(suffix);
					if (output.valid = age < memory.size() + 1)
						output.value = memory[memory.size() - age];
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

			#pragma region if "log"



			#pragma endregion

			#pragma region if "ln"



			#pragma endregion

			#pragma region if a constant



			#pragma endregion

			#pragma region if a number
			else {										//returns the operand as a float number
				if (output.valid = isfloat(operand))
					output.value = stof(operand);
				return output;
			}
			#pragma endregion

		}
		//recursively executes the operation
		out_t execop(const out_t& o_left, const std::string& op, const std::string& right) {
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
				out.value = powf(out.value, 1 / o_left.value);
			else if (op == "log")
				out.value = log10f(out.value);
			else if (op == "ln")
				out.value = logf(out.value);
			else {
				out.valid = false;
				return out;
			}
			#pragma endregion

			if (op == "log" || op == "ln") {
				if (exec_point != tokens.begin() && !isoperator(*(exec_point - 1)))
					tokens.insert(exec_point - 1, "*");
				*(exec_point) = std::to_string(out.value);
				tokens.erase(exec_point + 1, exec_point + 2);
				exec_point = nextop();
			}
			else {
				*(exec_point - 1) = std::to_string(out.value);
				tokens.erase(exec_point, exec_point + 2);
				exec_point = nextop();
			}

			if (exec_point == tokens.end())
				return out;
			else if (*exec_point != "log" && *exec_point != "ln")
				return execop(strtoop(*(exec_point - 1)), *exec_point, *(exec_point + 1));
			else
				return execop(out_t(), *exec_point, *(exec_point + 1));			
		}

	public:
		//starts the execution of an expression
		out_t startex(const std::string& exp) {
			out_t out;
			
			try {
				tokens = dividetokens(exp);
				exec_point = nextop();
				if (exec_point == tokens.end())
					out = strtoop(*(exec_point - 1));
				else if (exec_point != tokens.end() - 1)
					if (*exec_point == "log" || *exec_point == "ln")
						out = execop(out_t(), *exec_point, *(exec_point + 1));
					else
						out = execop(strtoop(*(exec_point - 1)), *exec_point, *(exec_point + 1));
				else
					out.valid = false;
			}
			catch (...) {
				out.valid = false;
			}

			if (out.valid)
				memory.push_back(out.value);
			tokens.erase(tokens.begin(), tokens.end());

			return out;
		}
		//return whether a string contains a valid signed float number
		bool isfloat(const std::string& s) {
			bool dot = false;
			for (int i = 0; i < s.length(); i++)
				if (!isdigit(s[i]) && s[i] != '.' && i == 0 && s[i] != '-' && s[i] != '+')
					return false;
				else if (s[i] == '.' && !dot)
					dot = true;
				else if (s[i] == '.' && dot)
					return false;
			return true;
		}
		//return whether a string contains a valid unsigned integer number
		bool isunsint(const std::string& s) {
			for (int i = 0; i < s.length(); i++)
				if (!isdigit(s[i]))
					return false;
			return true;
		}
		//return whether a string is a valid operator
		bool isoperator(const std::string& s) {
			for (int i = 0; i < ops_pr_1.size(); i++)
				if (s == ops_pr_1.at(i))
					return true;
			for (int i = 0; i < ops_pr_2.size(); i++)
				if (s == ops_pr_2.at(i))
					return true;
			for (int i = 0; i < ops_pr_3.size(); i++)
				if (s == ops_pr_3.at(i))
					return true;
			return false;
		}

		typedef out_t out_type;
	};
}