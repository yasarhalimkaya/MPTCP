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
#include <sstream>

int main(int argc, char* argv[]) {

	// Parse arguments
	if (argc < 3) {
		std::cout << "Specify at least two server interface addresses and ports" << std::endl;
		return 0;
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

	// TODO: Remove before submission
	Connection connection(servers.at(0));
	//Connection connection2(servers.at(1));
	
	FileListRequest fileListRequest;
	connection.send(fileListRequest);

	FileListResponse fileListResponse;
	connection.recv(fileListResponse);

	uint8_t numberOfFiles = fileListResponse.getNumberOfFiles();
	std::cout << "Number of files : " << (int)numberOfFiles << std::endl;

	for (int i = 1; i < numberOfFiles; i++) {
		std::cout << "File id : " << (int)fileListResponse.getFileId(i) << " File name : " << fileListResponse.getFileName(i) << std::endl;
		std::cout << "Requesting size..." << std::endl;

		FileSizeRequest fileSizeRequest(fileListResponse.getFileId(i));
		connection.send(fileSizeRequest);

		FileSizeResponse fileSizeResponse;
		connection.recv(fileSizeResponse);

		std::cout << "Size received : " << fileSizeResponse.getFileSize()
				<< " for file with id : " << (int)fileSizeResponse.getFileId() << std::endl;

		std::cout << "Requesting some data..." << std::endl;
		FileDataRequest fileDataRequest(fileListResponse.getFileId(i), 1, fileSizeResponse.getFileSize());
		connection.send(fileDataRequest);

		FileDataResponse fileDataResponse;
		connection.recv(fileDataResponse);

		std::cout << "Data received for file with id : " << (int)fileDataResponse.getFileId()
				<< " startByte : " << fileDataResponse.getStartByte()
				<< " endByte : " << fileDataResponse.getEndByte()
				<< " dataSize : " << fileDataResponse.getDataSize() << std::endl;

		std::ofstream myfile(fileListResponse.getFileName(i));
		myfile << fileDataResponse.getData();
		myfile.close();

		std::stringstream ss;

#ifdef __APPLE__
		ss << "md5 " << fileListResponse.getFileName(i);
#elif __linux__
		ss << "md5sum " << fileListResponse.getFileName(i);
#endif

		system(ss.str().c_str());
	}

	//connection2.send(fileListRequest);

	return 0;
}
