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
		std::deque<float> memory;			//keeps every old result
		str_deque tokens;					//keeps every "word" of an expression
		str_deque ops_pr_1 { "log", "ln" };	//operators to execute first
		str_deque ops_pr_2 { "^", "v" };	//operators to execute first
		str_deque ops_pr_3 { "*", "/" };	//operators to execute second
		str_deque ops_pr_4 { "+", "-" };	//operators to execute last
		str_deque::iterator exec_point;		//keeps track of which operator is being executed

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

			size_t beg_off = snip_begin - tokens.begin();
			size_t end_off = snip_end - tokens.begin();
			if (snip_begin != tokens.begin() && !isoperator(*(snip_begin - 1))) {
				tokens.insert(snip_begin, "*");
				beg_off++;
				end_off++;
				snip_begin = tokens.begin() + beg_off;
				snip_end = tokens.begin() + end_off;
			}
			if (snip_end < tokens.end() - 1 && !isoperator(*(snip_end + 1))) {
				tokens.insert(snip_end + 1, "*");
				snip_begin = tokens.begin() + beg_off;
				snip_end = tokens.begin() + end_off;
			}

			if (snip_end - snip_begin < 5) {
				if (*snip_begin == "(") {
					tokens.erase(snip_begin);
					end_off--;
				}
				if (tokens.begin() + end_off != tokens.end())
					tokens.erase(tokens.begin() + end_off);
				snip_begin = tokens.begin() + beg_off;
				snip_end = tokens.begin() + end_off;
			}

			out = std::find_first_of(snip_begin, snip_end, ops_pr_1.begin(), ops_pr_1.end());
			if (out == snip_end) {
				out = std::find_first_of(snip_begin, snip_end, ops_pr_2.begin(), ops_pr_2.end());
				if (out == snip_end) {
					out = std::find_first_of(snip_begin, snip_end, ops_pr_3.begin(), ops_pr_3.end());
					if (out == snip_end)
						out = std::find_first_of(snip_begin, snip_end, ops_pr_4.begin(), ops_pr_4.end());
				}
			}

			return out;
		}
		//recursively converts an "operand" (which includes r_{num}, sqrt_{num}, etc...) to real numbers kept in the struct out_t
		out_t strtoop(const std::string& operand) {
			out_t out;
			size_t div = operand.find_first_of('_');
			std::string prefix = operand.substr(0, div);
			std::string suffix = div < operand.length() ? operand.substr(div + 1) : operand;

			//checks the prefix for all the preset ones

			#pragma region if "r"
			if (prefix == "r") {						//r_{num} returns number of num age from memory
				if (out.valid = isunsint(suffix)) {
					size_t age = stoi(suffix);
					if (out.valid = age < memory.size() + 1)
						out.value = memory[memory.size() - age];
				}
			}
			#pragma endregion

			#pragma region if "sqrt"
			else if (prefix == "sqrt") {				//sqrt_{num} square root of num
				out_t num = strtoop(suffix);
				if (out.valid = num.valid)
					out.value = sqrtf(num.value);
			}
			#pragma endregion

			#pragma region if "sqr"
			else if (prefix == "sqr") {					//sqr_{num}	square of num
				out_t num = strtoop(suffix);
				if (out.valid = num.valid)
					out.value = num.value * num.value;
			}
			#pragma endregion

			#pragma region if "log"
			else if (prefix == "log") {					//sqr_{num}	square of num
				out_t num = strtoop(suffix);
				if (out.valid = num.valid)
					out.value = log10f(num.value);
			}
			#pragma endregion

			#pragma region if "ln"
			else if (prefix == "ln") {					//sqr_{num}	square of num
				out_t num = strtoop(suffix);
				if (out.valid = num.valid)
					out.value = logf(num.value);
			}
			#pragma endregion

			#pragma region if a constant
			else if (prefix == "pi") {
				if (out.valid = operand == "pi")
					out.value = 3.1427f;
			}
			else if (prefix == "e") {
				if (out.valid = operand == "e")
					out.value = 2.7182f;
			}
			#pragma endregion

			#pragma region if a number
			else {										//returns the operand as a float number
				if (out.valid = isfloat(operand))
					out.value = stof(operand);
			}
			#pragma endregion

			return out;
		}
		//executes an operation which takes two numbers
		out_t execute(const std::string& left, const std::string& op, const std::string& right) {
			out_t o_left = strtoop(left);
			out_t o_right = strtoop(right);
			out_t out;

			if (!(out.valid = o_left.valid && o_right.valid))
				return out;

			#pragma region operations
			if (op == "+")
				out.value = o_left.value + o_right.value;
			else if (op == "-")
				out.value = o_left.value - o_right.value;
			else if (op == "*")
				out.value = o_left.value * o_right.value;
			else if (op == "/")
				out.value = o_left.value / o_right.value;
			else if (op == "^")
				out.value = powf(o_left.value, o_right.value);
			else if (op == "v")
				out.value = powf(o_right.value, 1 / o_left.value);
			else {
				out.valid = false;
				return out;
			}
			#pragma endregion

			*(exec_point - 1) = std::to_string(out.value);
			tokens.erase(exec_point, exec_point + 2);
			exec_point = nextop();
			if (exec_point == tokens.end())
				return out;
			else if (*exec_point == "log" || *exec_point == "ln")
				return execute(*exec_point, *(exec_point + 1));
			else
				return execute(*(exec_point - 1), *exec_point, *(exec_point + 1));
		}
		//executes an operation which takes one number
		out_t execute(const std::string& op, const std::string& right) {
			out_t o_right = strtoop(right);
			out_t out;

			if (!(out.valid = o_right.valid))
				return out;

			#pragma region operations
			if (op == "log")
				out.value = log10f(o_right.value);
			else if (op == "ln")
				out.value = logf(o_right.value);
			else {
				out.valid = false;
				return out;
			}
			#pragma endregion

			*(exec_point) = std::to_string(out.value);
			tokens.erase(exec_point + 1, exec_point + 2);
			exec_point = nextop();
			if (exec_point == tokens.end())
				return out;
			else if (*exec_point == "log" || *exec_point == "ln")
				return execute(*exec_point, *(exec_point + 1));
			else
				return execute(*(exec_point - 1), *exec_point, *(exec_point + 1));
		}

	public:
		//starts the execution of an expression
		out_t startex(const std::string& exp) {
			out_t out;
			
			try {
				tokens = dividetokens(exp);
				exec_point = nextop();
				if (out.valid = exec_point != tokens.end() && exec_point != tokens.end() - 1)
					if (*exec_point == "log" || *exec_point == "ln")
						out = execute(*exec_point, *(exec_point + 1));
					else if (out.valid = exec_point != tokens.begin())
						out = execute(*(exec_point - 1), *exec_point, *(exec_point + 1));
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