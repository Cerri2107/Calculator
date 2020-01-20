#pragma once
#include <deque>
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
		typedef std::deque<std::string> deck_str;

	private:
		deck<float> memory;										//keeps every old result
		std::deque<std::string> operations;						//keeps every part of an operation
		std::deque<std::string> ops_pr_1{ "*", "/", "^", "v" }; //operators to execute first
		std::deque<std::string> ops_pr_2{ "+", "-" };			//operators to execute second
		std::deque<std::string>::iterator op_count;				//keeps track of which operator to execute

		//returns iterator reference of next operation to execute
		std::deque<std::string>::iterator nextop();
		//recursively converts an "operand" (which includes r_{num}, sqrt_{num}, etc...) to real numbers kept in the struct out_t
		out_t strtoop(const std::string& operand);
		//recursively executes the operation
		out_t execop(const out_t& o_left, const std::string& op, std::string& right);

	public:
		//starts the execution of a line of operations
		out_t startop(std::string& line);
		//return whether a string contains a valid float number
		bool isFloat(const std::string& s);
		//return whether a string contains a valid integer number
		bool isInt(const std::string& s);
	};
}