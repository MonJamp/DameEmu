#include "Dissassembler.h"
#include <iostream>

int main() {
	std::string ROM_DIR;
	std::string OUTPUT_DIR;
	
	std::cout << "Enter ROM directory: ";
	std::getline(std::cin, ROM_DIR);

	std::cout << "Output file: ";
	std::getline(std::cin, OUTPUT_DIR);
	
	Dissassembler dissassembler(ROM_DIR);
	//dissassembler.PrintInstructions();
	dissassembler.DisassembleToFile(OUTPUT_DIR);

	return 0;
}