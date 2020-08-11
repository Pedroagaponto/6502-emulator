#include <iostream>

#include "cpu6502.h"
#include "utils.h"

int main( int argc, char *argv[ ] ) {
	Cpu6502 cpu;

	Utils::load_ROM(cpu.bus, argv[1]);

	for (int i = 0; i < 4; i++) cpu.clock();
}
