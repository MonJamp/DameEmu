#include "DameEmu.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#define debug_msg(x) cout << x
#else
#define debug_msg(x)
#endif

void DameEmu::UNKNOWN(uint8_t opcode) {
	debug_msg("Unknown OP: " << (int)opcode << endl);
}

void DameEmu::LDSP(uint16_t nn) {
	debug_msg("LD SP, " << nn << endl);
	SP = nn;
	PC += 3;
}