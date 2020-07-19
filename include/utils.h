#pragma once

#include <array>
#include <iomanip>
#include <iostream>
#include <string>

#include "log.h"

#define MEMORY_SIZE 0x1FFF

class Utils {
   public:
	static void load_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
	static void load_formatted_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);

	static void store_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
	static void store_formatted_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file, uint16_t begin = 0,
									 uint16_t end = MEMORY_SIZE);
};