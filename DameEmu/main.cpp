#include "DameEmu.h"

int main(int argc, char** argv) {
	DameEmu* dameEmu = new DameEmu(argv[1], argv[2]);
	while (dameEmu->Cycle() != HALT);
	system("pause");
	return 0;
}