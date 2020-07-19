#include "utils.h"

void Utils::load_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file) {
	std::ifstream ifs(file, std::ifstream::binary);

	if (ifs) {
		ifs.read(reinterpret_cast<char*>(&m[0]), m.size());
	}
}

void Utils::store_binary_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file) {
	std::ofstream ofs(file, std::ofstream::binary);

	if (ofs) {
		ofs.write(reinterpret_cast<char*>(&m[0]), m.size());
	}
}

void Utils::store_formatted_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file, uint16_t begin, uint16_t end) {
	std::ofstream ofs(file, std::ofstream::out);

	if (!ofs) {
		return;
	}

	// normalize index
	begin = begin - (begin % 8);
	end   = end + (end % 8) == 0 ? 0 : end + 8 - end % 8;

	ofs << std::hex << std::setfill('0') << std::uppercase;

	for (uint16_t i = begin; i < end;) {
		ofs << ":" << std::setw(4) << i << " ";
		uint16_t endl = i + 8;

		for (; i < endl; i++) {
			ofs << std::setw(2) << static_cast<int>(m[i]);
			if (i != endl - 1) ofs << " ";
		}
		ofs << "\n";
	}
}

void Utils::load_formatted_dump(std::array<uint8_t, MEMORY_SIZE>& m, std::string file) {
	std::ifstream ifs(file, std::ifstream::in);

	if (!ifs) {
		return;
	}
	ifs >> std::hex;
	while (ifs.peek() != std::ifstream::traits_type::eof()) {
		int addr;
		ifs.ignore(1, ':');
		ifs >> addr;
		for (uint16_t i = addr; i < addr + 8; i++) {
			int value;
			ifs >> value;
			m[i] = value;
		}
		ifs.ignore(1, ' ');
		ifs.ignore(1, '\n');
	}
}