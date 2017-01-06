#include "Connection.hpp"
#include "FileListRequest.hpp"
#include "FileSizeRequest.hpp"
#include "FileDataRequest.hpp"
#include "FileListResponse.hpp"
#include "FileSizeResponse.hpp"
#include "FileDataResponse.hpp"
#include "Downloader.hpp"
#include "DeltaTimer.hpp"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>

bool getReliableResponse(std::vector<Connection> &, Request &, Response &);

int main(int argc, char* argv[]) {

	// Parse arguments
	if (argc < 3) {
		std::cout << "Specify at least two server interface addresses and ports" << std::endl;
		return -1;
	}

	std::vector<ServerConf> servers;

	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);

		size_t delPos = arg.find(":");

		if (delPos == std::string::npos) {
			std::cout << "Not a valid parameter " << arg << std::endl;
			continue;
		}

		ServerConf serverConf;

		serverConf.ipAddr = arg.substr(0, delPos);
		serverConf.port = strtol(arg.substr(delPos+1).c_str(), NULL, 10);

		servers.push_back(serverConf);
	}

	if (servers.size() < 2) {
		std::cout << "There is not enough valid server addresses" << std::endl;
		return -1;
	}

	// Create connections
	std::vector<Connection> connections;

	for (int i = 0; i < servers.size(); i++) {
		connections.push_back(Connection(servers.at(i)));
	}

	// Set timeout
	for (int i = 0; i < connections.size(); i++) {
		if (!connections.at(i).setTimeout(RECV_TIMEOUT)) {
			std::cout << "Failed to set a timeout" << std::endl;
		}
	}

	// Get file list
	FileListRequest fileListRequest;
	FileListResponse fileListResponse;

	if (!getReliableResponse(connections, fileListRequest, fileListResponse)) {
		return -1;
	}

	while(true) {
		std::cout << "File List:" << std::endl;

		for (int i = 0; i < fileListResponse.getNumberOfFiles(); i++) {
			std::cout << (int)fileListResponse.getFileId(i) << "	" << fileListResponse.getFileName(i) << std::endl;
		}

		std::cout << "Enter a number: ";
		std::string input;
		std::cin >> input;

		int32_t fileId;
		if ((fileId = strtol(input.c_str(), NULL, 10)) == 0 && input.c_str()[0] != '0') {
			continue;
		}

		if (fileId == -1) {
			std::cout << "Quitting..." << std::endl;
			break;
		}

		std::cout << "File " << fileId << " has been selected. Getting the size information..." << std::endl;

		FileSizeRequest fileSizeRequest(fileId);
		FileSizeResponse fileSizeResponse;
		if (!getReliableResponse(connections, fileSizeRequest, fileSizeResponse)) {
			continue;
		}

		std::cout << "File " << fileId << " is " << fileSizeResponse.getFileSize() << " bytes. Starting to download..." << std::endl;

		// Download
		Downloader downloader(connections, fileId, fileListResponse.getFileNameById(fileId), fileSizeResponse.getFileSize());

		DeltaTimer timer;
		timer.start();
		bool downloaded = downloader.start();
		timer.stop();

		if (downloaded) {
			std::cout << "File " << fileId << " has been downloaded in " << timer.getDeltaMSec() << "ms" << std::endl;
			md5sum(fileListResponse.getFileNameById(fileId));
		} else {
			std::cout << "Download failed!" << std::endl;
		}
	}

	// Tear down Connections to shutdown allocated sockets
	for (int i = 0; i < connections.size(); i++) {
		connections.at(i).teardown();
	}

	return 0;
}


bool getReliableResponse(std::vector<Connection> & conns, Request & req, Response & res) {

	bool retVal = false;
	uint32_t failedCount = 0;

	while (true) {

		if (failedCount >= 3*conns.size()) {
			std::cout << "Failed to get a response from any server, try again" << std::endl;
			retVal = false;
			break;
		} else if (retVal) {
			break;
		}

		// Try all interfaces
		for (int i = 0; i < conns.size(); i++) {
			if (!conns.at(i).send(req)) {
				failedCount++;
				continue;
			}

			Connection::RECV_STATUS recvStatus = conns.at(i).recv(res);
			if (Connection::RECV_SUCCESS != recvStatus) {
				failedCount++;
				continue;
			}

			if (!res.isValid()) {
				failedCount++;
				continue;
			}

			retVal = true;
			break;
		}
	}

	return retVal;
}
