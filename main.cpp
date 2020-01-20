#include <iostream>
#include <string>
#include "calculator.cpp"


int main() {
    cerri::calculator calc;
	cerri::calculator::out_t out;
    std::string input;
    do {
        std::cout << "Write a sequence of operations: ";
        getline(std::cin, input);
        if (input == "exit")
            break;
		if (input != "") {
			out = calc.startop(input);
        	if (out.valid)
				std::cout << "\t\t\tresult: " << out.value;
        	else
				std::cout << "invalid operation";
			std::cout << std::endl << "-------------------------------" << std::endl;
		}

    } while(true);
}