#pragma once

#define LOG_DIR "./"
#define LOG_LEVEL LOG_LEVEL_TRACE

#include <fstream>
#include <iostream>
#include <string>

/**
 * Possible values for the level (granularity) of the logging.
 */
typedef enum {
	LOG_LEVEL_TRACE = 0,
	LOG_LEVEL_DEBUG = 1,
	LOG_LEVEL_WARN	= 2,
	LOG_LEVEL_INFO	= 3,
	LOG_LEVEL_ERROR = 4
} LogLevel;

class Log {
   public:
	static Log* getInstance();

   private:
	static Log*	  instance;
	LogLevel	  level;
	std::ofstream os;

	Log();
	~Log();

	void _error(const std::string msg);
	void _info(const std::string msg);

	void _debug(const std::string msg);
	void _warn(const std::string msg);

	void _trace(const std::string msg);

   public:
	inline void error(const std::string msg) {
		if (LOG_LEVEL_ERROR >= level) _error(msg);
	}

	void info(const std::string msg) {
		if (LOG_LEVEL_INFO >= level) _info(msg);
	}

	void debug(const std::string msg) {
		if (LOG_LEVEL_WARN >= level) _debug(msg);
	}
	void warn(const std::string msg) {
		if (LOG_LEVEL_DEBUG >= level) _warn(msg);
	}

	void trace(const std::string msg) {
		if (LOG_LEVEL_TRACE >= level) _trace(msg);
	}
};
