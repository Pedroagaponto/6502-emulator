#include <iostream>

#include "cpu6502.h"

int main() {
	Cpu6502 cpu;

	for (int i = 0; i < 4; i++) cpu.clock();
}
