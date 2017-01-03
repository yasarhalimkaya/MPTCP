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
	ss << "md5 " << filePath;
#elif __linux__
	ss << "md5sum " << filePath;
#endif

	if (-1 == system(ss.str().c_str())) {
		return false;
	}

	return true;
}

#endif /* COMMON_HPP_ */
