#include "Connection.hpp"
#include "FileListRequest.hpp"
#include "FileSizeRequest.hpp"
#include "FileDataRequest.hpp"
#include "FileListResponse.hpp"
#include "FileSizeResponse.hpp"

#include <vector>
#include <cstring>
#include <cstdlib>

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

	for (int i = 0; i < numberOfFiles; i++) {
		std::cout << "File id : " << (int)fileListResponse.getFileId(i) << " File name : " << fileListResponse.getFileName(i) << std::endl;
		std::cout << "Requesting size..." << std::endl;

		FileSizeRequest fileSizeRequest(fileListResponse.getFileId(i));
		connection.send(fileSizeRequest);

		FileSizeResponse fileSizeResponse;
		connection.recv(fileSizeResponse);

		std::cout << "Size received : " << fileSizeResponse.getFileSize() << std::endl;
	}

	//connection2.send(fileListRequest);

	return 0;
}
