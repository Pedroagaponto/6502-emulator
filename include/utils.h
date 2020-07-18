#pragma once

#include <array>
#include <iostream>
#include <string>

#include "log.h"


#defime MEMORY_SIZE 0x1FFF


class Utils {
	public:
		static void load_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
		static void load_formated_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);

		static void store_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
		static void store_formated_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file);
}