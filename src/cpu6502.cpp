#include "cpu6502.h"

Cpu6502::Cpu6502()
{
    lookup = {
        {0x00, "BRK", Cpu6502::brk, Cpu6502::implied,    7},
        {0x01, "ORA", Cpu6502::ora, Cpu6502::indirect_x, 6},
        {0x02, "ERR", Cpu6502::err, Cpu6502::implied,    0},
        {0x03, "ERR", Cpu6502::err, Cpu6502::implied,    0},
        {0x04, "ERR", Cpu6502::err, Cpu6502::implied,    0},
        {0x05, "ORA", Cpu6502::ora, Cpu6502::zero_page,  3},
        {0x06, "ASL", Cpu6502::asl, Cpu6502::zero_page,  5},
        {0x07, "ERR", Cpu6502::err, Cpu6502::implied,    0},
        {0x08, "PHP", Cpu6502::php, Cpu6502::implied,    3},
        {0x09, "ORA", Cpu6502::ora, Cpu6502::immidiate,  2}
    };


    bus[0] = 0x09;
    bus[1] = 0x02;
    bus[2] = 0x09;
    bus[3] = 0x01;

}

Cpu6502::Clock()
{
    //TODO check interrupt

    if (clock_counter != 0) {
        clock_counter--;
        return;
    }

    uint8_t op = bus[pc];

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
