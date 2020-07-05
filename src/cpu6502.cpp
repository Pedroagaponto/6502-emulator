#include "cpu6502.h"

Cpu6502::Cpu6502()
{
    lookup = {
        Instruction{0x00, "BRK", std::bind(&Cpu6502::brk, this), std::bind(&Cpu6502::implied, this),    7},
        Instruction{0x01, "ORA", std::bind(&Cpu6502::ora, this), std::bind(&Cpu6502::indirect_x, this), 6},
        Instruction{0x02, "ERR", std::bind(&Cpu6502::err, this), std::bind(&Cpu6502::implied, this),    0},
        Instruction{0x03, "ERR", std::bind(&Cpu6502::err, this), std::bind(&Cpu6502::implied, this),    0},
        Instruction{0x04, "ERR", std::bind(&Cpu6502::err, this), std::bind(&Cpu6502::implied, this),    0},
        Instruction{0x05, "ORA", std::bind(&Cpu6502::ora, this), std::bind(&Cpu6502::zero_page, this),  3},
        Instruction{0x06, "ASL", std::bind(&Cpu6502::asl, this), std::bind(&Cpu6502::zero_page, this),  5},
        Instruction{0x07, "ERR", std::bind(&Cpu6502::err, this), std::bind(&Cpu6502::implied, this),    0},
        Instruction{0x08, "PHP", std::bind(&Cpu6502::php, this), std::bind(&Cpu6502::implied, this),    3},
        Instruction{0x09, "ORA", std::bind(&Cpu6502::ora, this), std::bind(&Cpu6502::immidiate, this),  2}
    };


    bus[0] = 0x09;
    bus[1] = 0x02;
    bus[2] = 0x09;
    bus[3] = 0x01;

	clock_counter = 0;
	pc = 0;


}

void Cpu6502::clock()
{
    //TODO check interrupt
	

	std::cout << "Accumulator: " << unsigned(a) << std::endl;

    if (clock_counter != 0) {
        clock_counter--;
        return;
    }

    uint8_t op = bus[pc];

	std::cout << "pc: " << pc << std::endl;
	std::cout << "Op: " << unsigned(op) << std::endl;

    Instruction instruction = lookup[op];
    clock_counter = instruction.cycles;

    clock_counter += instruction.addressing();
    instruction.function();

    //dump/log state

    clock_counter--;

}

//Address mode
uint8_t Cpu6502::implied()
{
    return 0;
}

uint8_t Cpu6502::indirect_x()
{
    return 0;
}

uint8_t Cpu6502::zero_page()
{
    return 0;
}

uint8_t Cpu6502::immidiate()
{
	m = bus[pc + 1];
    pc += 2;
    return 0;
}

//Instructions
void Cpu6502::brk()
{

}

void Cpu6502::ora()
{

    a = a | m;

    sr.bit.zero = a == 0x00;
    sr.bit.negative = a & 0x80;

}

void Cpu6502::err()
{

}

void Cpu6502::asl()
{

}

void Cpu6502::php()
{

}
