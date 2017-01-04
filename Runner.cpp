#include "Connection.hpp"
#include "FileListRequest.hpp"
#include "FileSizeRequest.hpp"
#include "FileDataRequest.hpp"
#include "FileListResponse.hpp"
#include "FileSizeResponse.hpp"
#include "FileDataResponse.hpp"
#include "Downloader.hpp"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <ctime>

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
		serverConf.port = atoi(arg.substr(delPos+1).c_str());

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

	// TODO : Consider packet loss, and using other servers if this one fails
	FileListRequest fileListRequest;
	connections.at(0).send(fileListRequest);

	FileListResponse fileListResponse;
	connections.at(0).recv(fileListResponse);

	while(true) {
		std::cout << "File List:" << std::endl;

		for (int i = 0; i < fileListResponse.getNumberOfFiles(); i++) {
			std::cout << (int)fileListResponse.getFileId(i) << "	" << fileListResponse.getFileName(i) << std::endl;
		}

		std::cout << "Enter a number: ";
		int32_t fileId;
		std::cin >> fileId;

		if (fileId == -1) {
			std::cout << "Quitting..." << std::endl;
			break;
		}

		std::cout << "File " << fileId << " has been selected. Getting the size information..." << std::endl;

		FileSizeRequest fileSizeRequest(fileId);
		connections.at(0).send(fileSizeRequest);

		FileSizeResponse fileSizeResponse;
		connections.at(0).recv(fileSizeResponse);

		if (!fileSizeResponse.isValid()) {
			if (fileSizeResponse.getResponseType() == Response::INVALID_FILE_ID) {
				std::cout << "Request failed with error INVALID_FILE_ID" << std::endl;
			}
		} else {
			std::cout << "File " << fileId << " is " << fileSizeResponse.getFileSize() << " bytes. Starting to download..." << std::endl;
		}

		// Download
		Downloader downloader(connections, fileId, fileListResponse.getFileNameById(fileId), fileSizeResponse.getFileSize());

		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC_RAW, &start);
		bool downloaded = downloader.start();
		clock_gettime(CLOCK_MONOTONIC_RAW, &end);

		uint64_t delta_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

		if (downloaded) {
			std::cout << "File " << fileId << " has been downloaded in " << delta_ms << "ms" << std::endl;
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
