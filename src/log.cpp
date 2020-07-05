#include <log.h>

Log* Log::instance = nullptr;
Log* Log::getInstance() {
	if (instance == nullptr) {
		instance = new Log();
	}

	return instance;
}

Log::Log() {
	level = LOG_LEVEL;
	os	= std::ofstream(std::string(LOG_DIR) + std::string("log.log"), std::ofstream::out);
}

Log::~Log() { os.close(); }

void Log::_trace(const std::string msg) {
	os << "TRACE: " << msg << std::endl;
	std::cout << msg << std::endl;
}

void Log::_debug(const std::string msg) {
	os << "DEBUG: " << msg << std::endl;
	std::cout << msg << std::endl;
}

void Log::_info(const std::string msg) {
	os << "INFO: " << msg << std::endl;
	std::cout << msg << std::endl;
}

void Log::_warn(const std::string msg) {
	os << "WARN: " << msg << std::endl;
	std::cout << msg << std::endl;
}

void Log::_error(const std::string msg) {
	os << "ERROR: " << msg << std::endl;
	std::cout << msg << std::endl;
}
