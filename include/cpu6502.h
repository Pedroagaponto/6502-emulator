#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <functional>

typedef struct {
    int8_t opcode;
    std::string name;
    std::function<void(void)> function;
    std::function<uint8_t(void)> addressing;
    uint8_t cycles;
} Instruction;


struct sr_modes {
    bool negative    : 1;
    bool overflow    : 1;
    bool ignored     : 1;
    bool break       : 1;
    bool decimal     : 1;
    bool interrupt   : 1;
    bool zero        : 1;
    bool carry       : 1;
};

typedef union {
    uint8_t full;
    struct sr_modes bit;
} Status;


class Cpu6502
{
public:

    void clock();


private:
    uint8_t a, x, y;
    uint16_t pc;
    uint8_t sp;
    Status sr;

    uint8_t m;

    std::array<Instruction, 0x09> lookup;
    std::array<uint8_t, 0x1FFF> bus;


    //Address mode
    uint8_t implied();
    uint8_t indirect_x();
    uint8_t zero_page();
    uint8_t immidiate();

    //Instructions
    void brk();
    void ora();
    void err();
    void asl();
}
