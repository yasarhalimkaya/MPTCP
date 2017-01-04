#ifndef DOWNLOADER_HPP_
#define DOWNLOADER_HPP_

#include "Connection.hpp"
#include <vector>

class Downloader {
public:
	Downloader(std::vector<Connection> & connections,
			   uint8_t fileId,
			   std::string fileName,
			   uint32_t fileSize);

	bool start();

	virtual ~Downloader();

private:
	std::vector<Connection> connections;
	uint8_t fileId;
	std::string fileName;
	uint32_t fileSize;
};

#endif /* DOWNLOADER_HPP_ */
