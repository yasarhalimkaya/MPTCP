#include "Connection.hpp"
#include <vector>

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
	// Print server confs for debug purposes
	for (int i = 0; i < servers.size(); i++) {
		std::cout << "Server " << i << " ip " << servers.at(i).ipAddr << " port " << servers.at(i).port << std::endl;
	}

	Connection connection(servers.at(0));

	char message[100] = "Message";

	connection.send(message);

	strcpy(message, "terminate");

	connection.send(message);

	return 0;
}
