#include "Downloader.hpp"
#include "FileDataRequest.hpp"
#include "FileDataResponse.hpp"
#include <fstream>

Downloader::Downloader(std::vector<Connection>& connections, uint8_t fileId,
		std::string fileName, uint32_t fileSize) {
	this->connections = connections;
	this->fileId = fileId;
	this->fileName = fileName;
	this->fileSize = fileSize;
}

bool Downloader::start() {
	// If the whole file can fit in a single packet, download with single request
	if (fileSize <= MAX_DATA_SIZE) {
		FileDataRequest fileDataRequest(fileId, 1, fileSize);
		connections.at(0).send(fileDataRequest);

		FileDataResponse fileDataResponse;
		connections.at(0).recv(fileDataResponse);

		if (fileDataResponse.isValid() && fileDataResponse.getDataSize() == fileSize) {
			std::ofstream file(fileName);
			file << fileDataResponse.getData();
			file.close();
			return true;
		} else {
			// TODO : Altough we set a MAX_DATA_SIZE, server's may not be the same
			return false;
		}
	}

	return false;
}

Downloader::~Downloader() {

}
