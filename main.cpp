#include <iostream>
#include <string>
#include "calculator.cpp"


int main() {
    cerri::calculator calc;
	cerri::calculator::out_type out;
    std::string input;
    do {
        std::cout << " Write an expression: ";
        getline(std::cin, input);
        if (input == "exit")
            break;
		if (input != "") {
			out = calc.startex(input);
        	if (out.valid)
				std::cout << "\t      result: " << out.value;
        	else
				std::cout << "\t      result: invalid expression";
			std::cout << std::endl << "---------------------" << std::endl;
		}

    } while(true);
}