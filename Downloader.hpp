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

	// Max bytes sent in single packet
	const uint32_t MAX_DATA_SIZE = 1000;
};

#endif /* DOWNLOADER_HPP_ */
