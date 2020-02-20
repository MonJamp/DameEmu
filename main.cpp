#include "DameEmu.h"

int main(int argc, char** argv) {
	DameEmu* dameEmu = new DameEmu(argv[1]);
	dameEmu->Run();
	return 0;
}