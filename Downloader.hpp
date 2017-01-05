#ifndef DOWNLOADER_HPP_
#define DOWNLOADER_HPP_

#include "Connection.hpp"
#include "FileDataResponse.hpp"
#include <vector>
#include <thread>

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

	std::vector<std::thread> threads;
	std::vector<bool> threadStatus;
	void threadLoop(Connection conn, uint32_t startByte, uint32_t endByte, uint32_t vectorIndex);

	std::vector<std::vector<FileDataResponse>> responses;

	typedef enum _STATE {
		SEND,
		RECEIVE,
		RECOVER,
		REORDER
	} STATE;
};

#endif /* DOWNLOADER_HPP_ */
