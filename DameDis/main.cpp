#include "Dissassembler.h"
#include <iostream>

int main() {
	std::string ROM_DIR;
	
	std::cout << "Enter ROM directory: ";
	std::getline(std::cin, ROM_DIR);
	
	Dissassembler dissassembler(ROM_DIR);
	dissassembler.PrintInstructions();

	return 0;
}