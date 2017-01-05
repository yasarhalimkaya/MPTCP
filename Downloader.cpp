#include "Downloader.hpp"
#include "FileDataRequest.hpp"
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

	if (connections.size() == 0) {
		return retVal;
	}

	// Distribute chunks and start the threads
	uint32_t remainder = fileSize % connections.size();
	uint32_t totalSize = fileSize - remainder;
	uint32_t sizePerThread = totalSize / connections.size();

	uint32_t threadStartByte = 1;
	for (int i = 0; i < connections.size(); i++) {
		// Push back an empty response vector for each thread
		responses.push_back(std::vector<std::vector<FileDataResponse>>());

		// Assign remaining bytes to first thread
		uint32_t threadEndByte = threadStartByte+sizePerThread-1;
		if (i == 0) {
			threadEndByte += remainder;
		}

		threads.push_back(std::thread(&Downloader::threadLoop, this, connections.at(i), threadStartByte, threadEndByte , i));

		threadStartByte = threadEndByte + 1;
	}

	// Wait for threads to finish and then join
	for (int i = 0; i < threads.size(); i++) {
		threads.at(i).join();
	}

	// Write to file
	std::ofstream file(fileName);
	for (int i = 0; i < responses.size(); i++) {
		if (responses.at(i).size() != 1) {
			std::cout << "Fragmented download" << std::endl;
			retVal = false;
		} else {
			for (int j = 0; j < responses.at(i).at(0).size(); j++) {
				file.write((const char*)responses.at(i).at(0).at(j).getData(), responses.at(i).at(0).at(j).getDataSize());
			}

			retVal = true;
		}
	}

	file.close();
	return retVal;
}

void Downloader::threadLoop(Connection conn, uint32_t startByte, uint32_t endByte, uint32_t vectorIndex) {
	FileDataRequest fileDataRequest(fileId, startByte, endByte);
	conn.send(fileDataRequest);

	while(true) {
		FileDataResponse fileDataResponse;
		conn.recv(fileDataResponse);

		if (!fileDataResponse.isValid()) {
			std::cout << "Invalid response" << std::endl;
			break;
		}

		addFileDataResponse(fileDataResponse, vectorIndex);

		if (fileDataResponse.getEndByte() == endByte) {
			break;
		}
	}
}

void Downloader::addFileDataResponse(FileDataResponse & response, uint32_t vectorIndex) {
	addResponseMutex.lock();

	int index = -1;
	for (int i = 0; i < responses.at(vectorIndex).size(); i++) {
		if (responses.at(vectorIndex).at(i).size() != 0 && (responses.at(vectorIndex).at(i).back().getEndByte() == response.getStartByte() - 1)) {
				index = i;
				break;
		}
	}

	if (index == -1) {
		if (responses.at(vectorIndex).size() != 0) {
			std::cout << "Packet loss detected, deal with it" << std::endl;
		}

		std::vector<FileDataResponse> newVec;
		newVec.push_back(response);
		responses.at(vectorIndex).push_back(newVec);
	} else {
		responses.at(vectorIndex).at(index).push_back(response);
	}

	addResponseMutex.unlock();
}

Downloader::~Downloader() {

}
