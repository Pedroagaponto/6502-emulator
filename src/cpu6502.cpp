#include "cpu6502.h"

void Cpu6502::clock() {
	// TODO check interrupt

	log->debug("Acc: " + std::to_string(a));

	if (clock_counter != 0) {
		clock_counter--;
		return;
	}

	uint8_t op = read_m(pc);

	log->debug("PC: " + std::to_string(pc));
	log->debug("OP: " + std::to_string(op));

	current_instruction = lookup[op];
	clock_counter		= current_instruction.cycles;

	// bus[bus[pc]]
	clock_counter += current_instruction.addressing();
	current_instruction.function();

	// dump/log state

	clock_counter--;
}

// Address mode
uint8_t Cpu6502::absolute_x() { return 0; }
uint8_t Cpu6502::absolute_y() { return 0; }
uint8_t Cpu6502::absolute() { return 0; }
uint8_t Cpu6502::accumulator() { return 0; }
uint8_t Cpu6502::immediate() {
	m = read_m(pc + 1);
	pc += 2;
	return 0;
}
uint8_t Cpu6502::implied() { return 0; }
uint8_t Cpu6502::indirect_x() { return 0; }
uint8_t Cpu6502::indirect() { return 0; }
uint8_t Cpu6502::indirect_y() { return 0; }
uint8_t Cpu6502::relative() { return 0; }
uint8_t Cpu6502::zero_page_x() { return 0; }
uint8_t Cpu6502::zero_page_y() { return 0; }
uint8_t Cpu6502::zero_page() { return 0; }

// Invalid address modes
uint8_t Cpu6502::indirect_z() { return 0; }
uint8_t Cpu6502::zero_page_r() { return 0; }
uint8_t Cpu6502::iax() { return 0; }

// Instructions
void Cpu6502::AND() {
	a = a & m;

	sr.bit.zero		= a == 0x00;
	sr.bit.negative = a & 0x80;
}
void Cpu6502::ADC() {
	uint16_t sum = m + (uint16_t)a + (uint16_t)sr.bit.carry;

	sr.bit.carry	= sum & 0xFF00;  // sum > 255
	sr.bit.overflow = (~(a ^ m) & (a ^ sum)) & 0x80;

	sr.bit.zero		= (sum & 0xFF) == 0;
	sr.bit.negative = a & 0x80;

	a = sum & 0xFF;
}
void Cpu6502::ASL() {
	bool carry = sr.bit.carry;

	a = a << 1;

	sr.bit.carry	= carry;
	sr.bit.overflow = carry;
	sr.bit.zero		= a == 0x00;
	sr.bit.negative = a & 0x80;
}
void Cpu6502::BBR() {}
void Cpu6502::BBS() {}
void Cpu6502::BCC() {
	if (sr.bit.carry == 0) {
		pc += m;
	}
}
void Cpu6502::BCS() {}
void Cpu6502::BEQ() {}
void Cpu6502::BIT() {
	sr.bit.zero		= (a & m) == 0;
	sr.bit.overflow = m & 0x40;
	sr.bit.negative = m & 0x80;
}
void Cpu6502::BLP() {}
void Cpu6502::BMI() {}
void Cpu6502::BNE() {}
void Cpu6502::BPL() {}
void Cpu6502::BRA() {}
void Cpu6502::BRK() {}
void Cpu6502::BVC() {}
void Cpu6502::BVS() {}
void Cpu6502::CLC() { sr.bit.carry = 0; }
void Cpu6502::CLD() { sr.bit.decimal = 0; }
void Cpu6502::CLI() { sr.bit.interrupt = 0; }
void Cpu6502::CLV() { sr.bit.overflow = 0; }
void Cpu6502::CMP() {
	int8_t sub = a - m;

	sr.bit.carry	= sub >= 0;
	sr.bit.zero		= sub == 0;
	sr.bit.negative = sub & 0x80;
}
void Cpu6502::CPX() {
	int8_t sub = a - m;

	sr.bit.carry	= sub >= 0;
	sr.bit.zero		= sub == 0;
	sr.bit.negative = sub & 0x80;
}
void Cpu6502::CPY() {
	int8_t sub = a - m;

	sr.bit.carry	= sub >= 0;
	sr.bit.zero		= sub == 0;
	sr.bit.negative = sub & 0x80;
}
void Cpu6502::DEC() {
	m = m - 1;

	sr.bit.zero		= m == 0;
	sr.bit.negative = m & 0x80;
}
void Cpu6502::DEX() {
	x--;

	sr.bit.zero		= x == 0;
	sr.bit.negative = x & 0x80;
}
void Cpu6502::DEY() {
	y--;

	sr.bit.zero		= y == 0;
	sr.bit.negative = y & 0x80;
}
void Cpu6502::EOR() {
	a = a ^ m;

	sr.bit.zero		= a == 0x00;
	sr.bit.negative = a & 0x80;
}
void Cpu6502::ERR() {}
void Cpu6502::INC() {
	m++;

	sr.bit.zero		= m == 0x00;
	sr.bit.negative = m & 0x80;
}
void Cpu6502::INX() {
	x++;

	sr.bit.zero		= x == 0x00;
	sr.bit.negative = x & 0x80;
}
void Cpu6502::INY() {
	y++;

	sr.bit.zero		= y == 0x00;
	sr.bit.negative = y & 0x80;
}
void Cpu6502::JMP() {
	// TODO: m vira ponteiro

	pc = m;
}
void Cpu6502::JMT() {}
void Cpu6502::JSR() {
	// TODO: m vira ponteiro
	pc--;
	write_m(sp--, pc >> 8); // TODO: hi or low first
	write_m(sp--, pc & 0xFF);

	pc = m;
}
void Cpu6502::LDA() {
	a = m;

	sr.bit.zero		= a == 0x00;
	sr.bit.negative = a & 0x80;
}
void Cpu6502::LDX() {
	x = m;

	sr.bit.zero		= x == 0x00;
	sr.bit.negative = x & 0x80;
}
void Cpu6502::LDY() {
	y = m;

	sr.bit.zero		= y == 0x00;
	sr.bit.negative = y & 0x80;
}
void Cpu6502::LSR() {
	sr.bit.carry = m & 0x01;

	m = m / 2;

	sr.bit.zero		= m == 0x00;
	sr.bit.negative = m & 0x80;
}
void Cpu6502::NOP() { return; }
void Cpu6502::ORA() {
	a = a | m;

	sr.bit.zero		= a == 0x00;
	sr.bit.negative = a & 0x80;
}
void Cpu6502::PHA() { write_m(sp--, a); }
void Cpu6502::PHP() { write_m(sp--, sr.full); }
void Cpu6502::PHX() {}
void Cpu6502::PHY() {}
void Cpu6502::PLA() {
	a = read_m(++sp);

	sr.bit.zero		= a == 0x00;
	sr.bit.negative = a & 0x80;
}
void Cpu6502::PLP() { sr.full = read_m(++sp); }
void Cpu6502::PLX() {}
void Cpu6502::PLY() {}
void Cpu6502::RMB() {}
void Cpu6502::ROL() {
	int carry = m == 0x80;

	m = m << 1 | sr.bit.carry;

	sr.bit.carry	= carry;
	sr.bit.zero		= m == 0x00;
	sr.bit.negative = m & 0x80;

	// TODO: if (&current_instruction.addresing == &Cpu6502::accumulator)
}
void Cpu6502::ROR() {
	// C -> [76543210] -> C
	int carry = m == 0x01;

	m = m >> 1 | (sr.bit.carry << 7);

	sr.bit.carry	= carry;
	sr.bit.zero		= m == 0x00;
	sr.bit.negative = m & 0x80;
}
void Cpu6502::RTI() {}
void Cpu6502::RTS() {
	pc = read_m(++sp);
	pc |= (uint16_t)read_m(++sp) << 8;
	pc++;
}
void Cpu6502::SBC() {
	/*
	uint16_t  = m + (uint16_t)a + (uint16_t)sr.bit.carry;

	sr.bit.carry	= sum & 0xFF00;  // sum > 255
	sr.bit.overflow = (~(a ^ m) & (a ^ sum)) & 0x80;

	sr.bit.zero		= (sum & 0xFF) == 0;
	sr.bit.negative = a & 0x80;

	a = sum & 0xFF;
	*/
	// uint16_t sub = (u_int16_t) a
}
void Cpu6502::SEC() { sr.bit.carry = 1; }
void Cpu6502::SED() { sr.bit.decimal = 0; }
void Cpu6502::SEI() { sr.bit.interrupt = 0; }
void Cpu6502::SMB() {}
void Cpu6502::STA() { m = a; }
void Cpu6502::STC() {}
void Cpu6502::STP() {}
void Cpu6502::STX() { m = x; }
void Cpu6502::STY() { m = y; }
void Cpu6502::STZ() {}
void Cpu6502::TAX() { x = a; }
void Cpu6502::TAY() { y = a; }
void Cpu6502::TRB() {}
void Cpu6502::TSB() {}
void Cpu6502::TSX() { x = sp; }
void Cpu6502::TXA() { a = x; }
void Cpu6502::TXS() { sp = x; }
void Cpu6502::TYA() { a = y; }
void Cpu6502::WAI() {}

// clang-format off
Cpu6502::Cpu6502() {
	lookup = {
		Instruction{0x00, "BRK",  std::bind(&Cpu6502::BRK, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 7},
		Instruction{0x01, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0x02, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x03, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x04, "TSB",  std::bind(&Cpu6502::TSB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // 65C02
		Instruction{0x05, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x06, "ASL",  std::bind(&Cpu6502::ASL, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},
		Instruction{0x07, "RMB0", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x08, "PHP",  std::bind(&Cpu6502::PHP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 3},
		Instruction{0x09, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x0A, "ASL",  std::bind(&Cpu6502::ASL, this), std::bind(&Cpu6502::accumulator, this), Addr::ACC, 2},
		Instruction{0x0B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x0C, "TSB",  std::bind(&Cpu6502::TSB, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},  // 65C02
		Instruction{0x0D, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x0E, "ASL",  std::bind(&Cpu6502::ASL, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0x0F, "BBR0", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x10, "BPL",  std::bind(&Cpu6502::BPL, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0x11, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0x12, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0x13, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x14, "TRB",  std::bind(&Cpu6502::TRB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // 65C02
		Instruction{0x15, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x16, "ASL",  std::bind(&Cpu6502::ASL, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 6},
		Instruction{0x17, "RMB1", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x18, "CLC",  std::bind(&Cpu6502::CLC, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x19, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0x1A, "INC",  std::bind(&Cpu6502::INC, this), std::bind(&Cpu6502::accumulator, this), Addr::ACC, 2},  // 65C02
		Instruction{0x1B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x1C, "TRB",  std::bind(&Cpu6502::TRB, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},  // 65C02
		Instruction{0x1D, "ORA",  std::bind(&Cpu6502::ORA, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0x1E, "ASL",  std::bind(&Cpu6502::ASL, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 7},
		Instruction{0x1F, "BBR1", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x20, "JSR",  std::bind(&Cpu6502::JSR, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0x21, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0x22, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x23, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x24, "BIT",  std::bind(&Cpu6502::BIT, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x25, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x26, "ROL",  std::bind(&Cpu6502::ROL, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},
		Instruction{0x27, "RMB2", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x28, "PLP",  std::bind(&Cpu6502::PLP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 4},
		Instruction{0x29, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x2A, "ROL",  std::bind(&Cpu6502::ROL, this), std::bind(&Cpu6502::accumulator, this), Addr::ACC, 2},
		Instruction{0x2B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x2C, "BIT",  std::bind(&Cpu6502::BIT, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x2D, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x2E, "ROL",  std::bind(&Cpu6502::ROL, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0x2F, "BBR2", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x30, "BMI",  std::bind(&Cpu6502::BMI, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0x31, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0x32, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0x33, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x34, "BIT",  std::bind(&Cpu6502::BIT, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},  // 65C02
		Instruction{0x35, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x36, "ROL",  std::bind(&Cpu6502::ROL, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 6},
		Instruction{0x37, "RMB3", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x38, "SEC",  std::bind(&Cpu6502::SEC, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x39, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0x3A, "DEC",  std::bind(&Cpu6502::DEC, this), std::bind(&Cpu6502::accumulator, this), Addr::ACC, 2},  // 65C02
		Instruction{0x3B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x3C, "BIT",  std::bind(&Cpu6502::BIT, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},  // 65C02
		Instruction{0x3D, "AND",  std::bind(&Cpu6502::AND, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0x3E, "ROL",  std::bind(&Cpu6502::ROL, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 7},
		Instruction{0x3F, "BBR3", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x40, "RTI",  std::bind(&Cpu6502::RTI, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 6},
		Instruction{0x41, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0x42, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x43, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x44, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x45, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x46, "LSR",  std::bind(&Cpu6502::LSR, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},
		Instruction{0x47, "RMB4", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x48, "PHA",  std::bind(&Cpu6502::PHA, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 3},
		Instruction{0x49, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x4A, "LSR",  std::bind(&Cpu6502::LSR, this), std::bind(&Cpu6502::accumulator, this), Addr::ACC, 2},
		Instruction{0x4B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},  // was N4B
		Instruction{0x4C, "JMP",  std::bind(&Cpu6502::JMP, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 3},
		Instruction{0x4D, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x4E, "LSR",  std::bind(&Cpu6502::LSR, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0x4F, "BBR4", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x50, "BVC",  std::bind(&Cpu6502::BVC, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0x51, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0x52, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},
		Instruction{0x53, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x54, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x55, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x56, "LSR",  std::bind(&Cpu6502::LSR, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 6},
		Instruction{0x57, "RMB5", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x58, "CLI",  std::bind(&Cpu6502::CLI, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x59, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0x5A, "PHY",  std::bind(&Cpu6502::PHY, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 3},
		Instruction{0x5B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x5C, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 8},
		Instruction{0x5D, "EOR",  std::bind(&Cpu6502::EOR, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0x5E, "LSR",  std::bind(&Cpu6502::LSR, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 7},
		Instruction{0x5F, "BBR5", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x60, "RTS",  std::bind(&Cpu6502::RTS, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 6},
		Instruction{0x61, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0x62, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x63, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x64, "STZ",  std::bind(&Cpu6502::STZ, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},  // 65C02
		Instruction{0x65, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x66, "ROR",  std::bind(&Cpu6502::ROR, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},
		Instruction{0x67, "RMB6", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x68, "PLA",  std::bind(&Cpu6502::PLA, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 4},
		Instruction{0x69, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x6A, "ROR",  std::bind(&Cpu6502::ROR, this), std::bind(&Cpu6502::accumulator, this), Addr::ACC, 2},
		Instruction{0x6B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x6C, "JMP",  std::bind(&Cpu6502::JMP, this), std::bind(&Cpu6502::indirect,    this), Addr::IND, 5},
		Instruction{0x6D, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x6E, "ROR",  std::bind(&Cpu6502::ROR, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0x6F, "BBR6", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x70, "BVS",  std::bind(&Cpu6502::BVS, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0x71, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0x72, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0x73, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x74, "STZ",  std::bind(&Cpu6502::STZ, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},  // 65C02
		Instruction{0x75, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x76, "ROR",  std::bind(&Cpu6502::ROR, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 6},
		Instruction{0x77, "RMB7", std::bind(&Cpu6502::RMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x78, "SEI",  std::bind(&Cpu6502::SEI, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x79, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0x7A, "PLY",  std::bind(&Cpu6502::PLY, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 4},
		Instruction{0x7B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x7C, "JMP",  std::bind(&Cpu6502::JMP, this), std::bind(&Cpu6502::iax,         this), Addr::IAX, 6},  // 65C02
		Instruction{0x7D, "ADC",  std::bind(&Cpu6502::ADC, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0x7E, "ROR",  std::bind(&Cpu6502::ROR, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 7},
		Instruction{0x7F, "BBR7", std::bind(&Cpu6502::BBR, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x80, "BRA",  std::bind(&Cpu6502::BRA, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 3},	  // 65C02
		Instruction{0x81, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0x82, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0x83, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x84, "STY",  std::bind(&Cpu6502::STY, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x85, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x86, "STX",  std::bind(&Cpu6502::STX, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0x87, "SMB0", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x88, "DEY",  std::bind(&Cpu6502::DEY, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x89, "BIT",  std::bind(&Cpu6502::BIT, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},  // 65C02
		Instruction{0x8A, "TXA",  std::bind(&Cpu6502::TXA, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x8B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x8C, "STY",  std::bind(&Cpu6502::STY, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x8D, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x8E, "STX",  std::bind(&Cpu6502::STX, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0x8F, "BBS0", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0x90, "BCC",  std::bind(&Cpu6502::BCC, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0x91, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 6},
		Instruction{0x92, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0x93, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x94, "STY",  std::bind(&Cpu6502::STY, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x95, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0x96, "STX",  std::bind(&Cpu6502::STX, this), std::bind(&Cpu6502::zero_page_y, this), Addr::ZPY, 4},
		Instruction{0x97, "SMB1", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0x98, "TYA",  std::bind(&Cpu6502::TYA, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x99, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 5},
		Instruction{0x9A, "TXS",  std::bind(&Cpu6502::TXS, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0x9B, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0x9C, "STZ",  std::bind(&Cpu6502::STZ, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},  // 65C02
		Instruction{0x9D, "STA",  std::bind(&Cpu6502::STA, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 5},
		Instruction{0x9E, "STZ",  std::bind(&Cpu6502::STZ, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 5},	// 65C02
		Instruction{0x9F, "BBS1", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0xA0, "LDY",  std::bind(&Cpu6502::LDY, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xA1, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0xA2, "LDX",  std::bind(&Cpu6502::LDX, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xA3, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xA4, "LDY",  std::bind(&Cpu6502::LDY, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xA5, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xA6, "LDX",  std::bind(&Cpu6502::LDX, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xA7, "SMB2", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0xA8, "TAY",  std::bind(&Cpu6502::TAY, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xA9, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xAA, "TAX",  std::bind(&Cpu6502::TAX, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xAB, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xAC, "LDY",  std::bind(&Cpu6502::LDY, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xAD, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xAE, "LDX",  std::bind(&Cpu6502::LDX, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xAF, "BBS2", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0xB0, "BCS",  std::bind(&Cpu6502::BCS, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0xB1, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0xB2, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0xB3, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xB4, "LDY",  std::bind(&Cpu6502::LDY, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0xB5, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0xB6, "LDX",  std::bind(&Cpu6502::LDX, this), std::bind(&Cpu6502::zero_page_y, this), Addr::ZPY, 4},
		Instruction{0xB7, "SMB3", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0xB8, "CLV",  std::bind(&Cpu6502::CLV, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xB9, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0xBA, "TSX",  std::bind(&Cpu6502::TSX, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xBB, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xBC, "LDY",  std::bind(&Cpu6502::LDY, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0xBD, "LDA",  std::bind(&Cpu6502::LDA, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0xBE, "LDX",  std::bind(&Cpu6502::LDX, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0xBF, "BBS3", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0xC0, "CPY",  std::bind(&Cpu6502::CPY, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xC1, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0xC2, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xC3, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xC4, "CPY",  std::bind(&Cpu6502::CPY, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xC5, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xC6, "DEC",  std::bind(&Cpu6502::DEC, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},
		Instruction{0xC7, "SMB4", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0xC8, "INY",  std::bind(&Cpu6502::INY, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xC9, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xCA, "DEX",  std::bind(&Cpu6502::DEX, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xCB, "WAI",  std::bind(&Cpu6502::WAI, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 3},  // WDC
		Instruction{0xCC, "CPY",  std::bind(&Cpu6502::CPY, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xCD, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xCE, "DEC",  std::bind(&Cpu6502::DEC, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0xCF, "BBS4", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0xD0, "BNE",  std::bind(&Cpu6502::BNE, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0xD1, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0xD2, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0xD3, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xD4, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0xD5, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0xD6, "DEC",  std::bind(&Cpu6502::DEC, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 6},
		Instruction{0xD7, "SMB5", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0xD8, "CLD",  std::bind(&Cpu6502::CLD, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xD9, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0xDA, "PHX",  std::bind(&Cpu6502::PHX, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 3},
		Instruction{0xDB, "STP",  std::bind(&Cpu6502::STP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 3},  // WDC
		Instruction{0xDC, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xDD, "CMP",  std::bind(&Cpu6502::CMP, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0xDE, "DEC",  std::bind(&Cpu6502::DEC, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 7},
		Instruction{0xDF, "BBS5", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0xE0, "CPX",  std::bind(&Cpu6502::CPX, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xE1, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::indirect_x,  this), Addr::INX, 6},
		Instruction{0xE2, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xE3, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xE4, "CPX",  std::bind(&Cpu6502::CPX, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xE5, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 3},
		Instruction{0xE6, "INC",  std::bind(&Cpu6502::INC, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},
		Instruction{0xE7, "SMB6", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0xE8, "INX",  std::bind(&Cpu6502::INX, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xE9, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::immediate,   this), Addr::IMM, 2},
		Instruction{0xEA, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xEB, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xEC, "CPX",  std::bind(&Cpu6502::CPX, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xED, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xEE, "INC",  std::bind(&Cpu6502::INC, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 6},
		Instruction{0xEF, "BBS6", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5},  // WDC
		Instruction{0xF0, "BEQ",  std::bind(&Cpu6502::BEQ, this), std::bind(&Cpu6502::relative,    this), Addr::REL, 2},
		Instruction{0xF1, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::indirect_y,  this), Addr::INY, 5},
		Instruction{0xF2, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::indirect_z,  this), Addr::INZ, 5},  // 65C02
		Instruction{0xF3, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xF4, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0xF5, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 4},
		Instruction{0xF6, "INC",  std::bind(&Cpu6502::INC, this), std::bind(&Cpu6502::zero_page_x, this), Addr::ZPX, 6},
		Instruction{0xF7, "SMB7", std::bind(&Cpu6502::SMB, this), std::bind(&Cpu6502::zero_page,   this), Addr::ZRP, 5},  // WDC
		Instruction{0xF8, "SED",  std::bind(&Cpu6502::SED, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 2},
		Instruction{0xF9, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::absolute_y,  this), Addr::ABY, 4},
		Instruction{0xFA, "PLX",  std::bind(&Cpu6502::PLX, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 4},
		Instruction{0xFB, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::implied,     this), Addr::IMP, 1},
		Instruction{0xFC, "NOP",  std::bind(&Cpu6502::NOP, this), std::bind(&Cpu6502::absolute,    this), Addr::ABS, 4},
		Instruction{0xFD, "SBC",  std::bind(&Cpu6502::SBC, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 4},
		Instruction{0xFE, "INC",  std::bind(&Cpu6502::INC, this), std::bind(&Cpu6502::absolute_x,  this), Addr::ABX, 7},
		Instruction{0xFF, "BBS7", std::bind(&Cpu6502::BBS, this), std::bind(&Cpu6502::zero_page_r, this), Addr::ZPR, 5}  // WDC
	};
	// clang-format on

	write_m(0, 0x09);
	write_m(1, 0x02);
	write_m(2, 0x09);
	write_m(3, 0x01);

	clock_counter = 0;
	pc			  = 0;

	log = Log::getInstance();
}

void Cpu6502::write_m(uint16_t address, uint8_t date) {
	bus[address] = date;
}

uint8_t Cpu6502::read_m(uint16_t address) {
	return bus[address];
}
