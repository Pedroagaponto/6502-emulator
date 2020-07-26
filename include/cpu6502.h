#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

#include "log.h"

typedef enum { ABY, ABX, ABS, ACC, IMM, IMP, INX, IND, INY, REL, ZPX, ZPY, ZRP, INZ, ZPR, IAX } Addr;

typedef struct {
	uint8_t						 opcode;
	std::string					 name;
	std::function<void(void)>	 function;
	std::function<uint8_t(void)> addressing;
	Addr						 addr;
	uint8_t						 cycles;
} Instruction;

struct sr_modes {
	bool negative : 1;
	bool overflow : 1;
	bool ignored : 1;
	bool brk : 1;
	bool decimal : 1;
	bool interrupt : 1;
	bool zero : 1;
	bool carry : 1;
};

typedef union {
	uint8_t			full;
	struct sr_modes bit;
} Status;

class Cpu6502 {
   private:
	uint8_t read_m(uint16_t address);
	void	write_m(uint16_t address, uint8_t data);

   public:
	Cpu6502();

	void clock();

   private:
	Log *log;

	uint8_t	 a, x, y;
	uint16_t pc;
	uint8_t	 sp;
	Status	 sr;

	uint8_t	 m;
	uint16_t addr;
	uint16_t tmp;

	Instruction current_instruction;

	std::array<Instruction, 0x100> lookup;
	std::array<uint8_t, 0x1FFF>	   bus;

	uint8_t clock_counter;

	// Address mode
	uint8_t absolute_x();
	uint8_t absolute_y();
	uint8_t absolute();
	uint8_t accumulator();
	uint8_t immediate();
	uint8_t implied();
	uint8_t indirect_x();
	uint8_t indirect();
	uint8_t indirect_y();
	uint8_t relative();
	uint8_t zero_page_x();
	uint8_t zero_page_y();
	uint8_t zero_page();

	// 65C02 only
	uint8_t indirect_z();
	uint8_t zero_page_r();
	uint8_t iax();

	// Instructions
	void ADC();
	void AND();
	void ASL();
	void BBR();
	void BBS();
	void BCC();
	void BCS();
	void BEQ();
	void BIT();
	void BLP();
	void BMI();
	void BNE();
	void BPL();
	void BRA();
	void BRK();
	void BVC();
	void BVS();
	void CLC();
	void CLD();
	void CLI();
	void CLV();
	void CMP();
	void CPX();
	void CPY();
	void DEC();
	void DEX();
	void DEY();
	void EOR();
	void ERR();
	void INC();
	void INX();
	void INY();
	void JMP();
	void JMT();
	void JSR();
	void LDA();
	void LDX();
	void LDY();
	void LSR();
	void NOP();
	void ORA();
	void PHA();
	void PHP();
	void PHX();
	void PHY();
	void PLA();
	void PLP();
	void PLX();
	void PLY();
	void RMB();
	void ROL();
	void ROR();
	void RTI();
	void RTS();
	void SBC();
	void SEC();
	void SED();
	void SEI();
	void SMB();
	void STA();
	void STC();
	void STP();
	void STX();
	void STY();
	void STZ();
	void TAX();
	void TAY();
	void TRB();
	void TSB();
	void TSX();
	void TXA();
	void TXS();
	void TYA();
	void WAI();
};