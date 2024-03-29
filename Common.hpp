#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <string>
#include <sstream>
#include <iostream>

typedef struct _ServerConf {
	std::string ipAddr;
	int port;
} ServerConf;

inline bool md5sum(std::string filePath) {
	std::stringstream ss;

#ifdef __APPLE__
	ss << "md5 '" << filePath << "'";
#elif __linux__
	ss << "md5sum '" << filePath << "'";
#endif

	if (-1 == system(ss.str().c_str())) {
		return false;
	}

	return true;
}

static const uint32_t MAX_DATA_SIZE = 1000;

/**
 * 1 byte request/response type
 * 1 byte file id
 * 4 byte start byte
 * 4 byte end byte
 */
static const uint32_t HEADER_SIZE = 10;

/**
 * Hardcoded receive timeout in usecs
 */
static const uint32_t RECV_TIMEOUT = 100000; // 100 ms

/**
 * Initial window size
 * Each connection has its own window size
 * changin according to its performance
 */
static const uint32_t WINDOW_SIZE = 1000;

/**
 *
 */
static const uint32_t MAX_WINDOW_SIZE = 1000000;

#endif /* COMMON_HPP_ */
