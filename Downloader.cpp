#include "Downloader.hpp"
#include "FileDataRequest.hpp"
#include <fstream>
#include <algorithm>

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

	uint32_t timeout = 100000; // 100ms
	uint32_t index = 1;
	std::vector<uint32_t> windowSizes;
	for (int i = 0; i < connections.size(); i++) {
		windowSizes.push_back(10000);
	}

	std::ofstream file(fileName);

	while (true) {
		if (index > fileSize) {
			retVal = true;
			break;
		}

		for (int i = 0; i < connections.size(); i++) {
			connections.at(i).setTimeout(timeout);

			uint32_t startByte = index;
			uint32_t endByte = index + windowSizes.at(i) - 1;

			if (fileSize < endByte) {
				endByte = fileSize;
			}

			responses.push_back(std::vector<FileDataResponse>());
			threadStatus.push_back(true);

			threads.push_back(std::thread(&Downloader::threadLoop,
										   this,
										   connections.at(i),
										   startByte,
										   endByte,
										   i));

			index += windowSizes.at(i);

			if (endByte == fileSize) {
				break;
			}
		}

		// Wait for threads to join
		for (int i = 0; i < threads.size(); i++) {
			threads.at(i).join();
		}

		// Clear threads
		threads.clear();

		// Write to file
		for (int i = 0; i < responses.size(); i++) {
			for (int j = 0; j < responses.at(i).size(); j++) {
				file.write((char*)responses.at(i).at(j).getData(), responses.at(i).at(j).getDataSize());
			}
		}
		
		// Clear responses
		responses.clear();
			
		// TODO
		// Check thread status and modify window size
		for (int i = 0; i < threadStatus.size(); i++) {
			if (threadStatus.at(i))
				windowSizes.at(i) *= 2;
			else
				windowSizes.at(i) /= 2;
		}

		threadStatus.clear();	
	}

	file.close();
	return retVal;
}

void Downloader::threadLoop(Connection conn, uint32_t startByte, uint32_t endByte, uint32_t index) {
	STATE state = SEND;
	uint32_t timeOutCount = 0;
	bool terminate = false;

	uint32_t localStartByte = startByte;
	uint32_t localEndByte = endByte;

	while(!terminate) {
		switch (state) {
			case SEND: {
				FileDataRequest fileDataRequest(fileId, localStartByte, localEndByte);
				conn.send(fileDataRequest);
				state = RECEIVE;
				break;
			}
			case RECEIVE: {
				while(true) {
					FileDataResponse response;
					Connection::RECV_STATUS status = conn.recv(response);

					if (status == Connection::RECV_TIMEOUT) {
						timeOutCount++;
						if (timeOutCount >= 3) {
							std::cout << "Connection timed out 3 times in a row in thread " << index << std::endl;
							break;
						}
					} else {
						timeOutCount = 0;
					}

					if (!response.isValid()) {
						continue;
					}

					responses.at(index).push_back(response);

					// TODO : Not that easy, broo
					if (response.getEndByte() == localEndByte) {
						break;
					}
				}

				state = RECOVER;
				break;
			}
			case RECOVER: {
				// Sort response array
				struct {
					bool operator()(FileDataResponse f1, FileDataResponse f2) {
						return f1.getStartByte() < f2.getStartByte();
					}
				} custom;

				std::sort(responses.at(index).begin(), responses.at(index).end(), custom);

				// Check if there is missing packages
				uint32_t dirty = 0;
				for (int i = 0; i < responses.at(index).size()-1; i++) {
					if (responses.at(index).at(i).getEndByte() != responses.at(index).at(i+1).getStartByte() - 1) {
						dirty++;
					}
				}

				if (dirty == 0 && responses.at(index).back().getEndByte() == endByte) {
					terminate = true;
				}
				else {
					std::cout << "Trying again in thread : " << index << std::endl;
					threadStatus.at(index) = false;
					state = SEND;
					responses.at(index).clear();
				}

				break;
			}
			default:
				break;
		}
	}
}

Downloader::~Downloader() {

}
