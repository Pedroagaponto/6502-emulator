#pragma once

#include <array>
#include <iomanip>
#include <iostream>
#include <string>

#include "log.h"

#define MEMORY_SIZE 0xFFFF
#define ROM_SIZE 0x7ff4
#define ROM_START_ADDR 0x8000

class Utils {
   public:
   	static void load_ROM(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
	static void load_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
	static void load_formatted_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);

	static void store_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
	static void store_formatted_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file, uint begin = 0,
									 uint end = MEMORY_SIZE);
};