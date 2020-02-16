#include "DameEmu.h"

int main(int argc, char** argv) {
	DameEmu* dameEmu = new DameEmu(argv[1]);
	while (dameEmu->Step() != HALT) {
		dameEmu->HandleInterrupts();
		dameEmu->UpdateScreen();
	}
	return 0;
}