#include "Connection.hpp"
#include "FileListRequest.hpp"
#include "FileSizeRequest.hpp"
#include "FileDataRequest.hpp"
#include "FileListResponse.hpp"
#include "FileSizeResponse.hpp"
#include "FileDataResponse.hpp"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>

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

	// TODO : Consider packet loss, and using other servers if this one fails
	Connection connection(servers.at(0));
	
	FileListRequest fileListRequest;
	connection.send(fileListRequest);

	FileListResponse fileListResponse;
	connection.recv(fileListResponse);

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
		connection.send(fileSizeRequest);

		FileSizeResponse fileSizeResponse;
		connection.recv(fileSizeResponse);

		if (!fileSizeResponse.isValid()) {
			if (fileSizeResponse.getResponseType() == Response::INVALID_FILE_ID) {
				std::cout << "Request failed with error INVALID_FILE_ID" << std::endl;
			}
		} else {
			std::cout << "File " << fileId << " is " << fileSizeResponse.getFileSize() << " bytes. Starting to download..." << std::endl;
		}

		// Download
		std::cout << "Downloading..." << std::endl;
		std::cout << "Download statistics..." << std::endl;
	}

	return 0;
}
