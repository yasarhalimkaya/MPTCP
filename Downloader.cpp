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
	bool retVal = false;
	std::ofstream file(fileName);

	// If the whole file can fit in a single packet, download with a single request
	if (fileSize <= MAX_DATA_SIZE) {
		FileDataRequest fileDataRequest(fileId, 1, fileSize);
		connections.at(0).send(fileDataRequest);

		while(true) {
			FileDataResponse fileDataResponse;
			connections.at(0).recv(fileDataResponse);

			if (!fileDataResponse.isValid()) {
				std::cout << "Invalid response" << std::endl;
				retVal = false;
				break;
			}

			file.write((const char*)fileDataResponse.getData(), fileDataResponse.getDataSize());

			if (fileDataResponse.getEndByte() == fileSize) {
				retVal = true;
				break;
			}
		}
	}

	file.close();
	return retVal;
}

Downloader::~Downloader() {

}
