#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>

/**
 * @summary A dummy server which listens
 * 			to given port number for UDP connections
 * 			and echoes the incoming data
 *
 * @param argv[1] port number
 */
int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cout << "Specify the port" << std::endl;
		return 0;
	}

	int port = atoi(argv[1]);

	// Create a UDP socket for localhost
	int sockFd = socket(AF_INET, SOCK_DGRAM, 0);

	if (!sockFd) {
		std::cout << "Failed to create a socket" << std::endl;
		goto TERMINATE;
	}

	// Create sockaddr_in structure for server
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET; // Localhost
	serverAddr.sin_port = port;
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket to given address
	if (bind(sockFd, (const sockaddr *)&serverAddr, sizeof(serverAddr)) != 0) {
		std::cout << "Failed to bind the socket to given address" << std::endl;
		goto TERMINATE;
	}

	// Listen incoming messages
	// As this is a UDP socket, there is no listen() or accept() calls
	struct sockaddr_in clientAddr; // Will be filled with the address of incoming message owners
	char messageBuffer[100]; // Will hold the incoming data

	while(true) {
		memset(messageBuffer, 0, sizeof(messageBuffer));

		socklen_t clientAddrLen = sizeof(clientAddr);
		int n = recvfrom(sockFd, messageBuffer, sizeof(messageBuffer), 0, (sockaddr *)&clientAddr, &clientAddrLen);

		if (n <= 0) {
			std::cout << "No data received, listen again" << std::endl;
			continue;
		}

		std::cout << "Received data : " << messageBuffer << std::endl;

		if (strcmp(messageBuffer, "terminate") == 0) {
			break;
		}
	}

	std::cout << "Shutting down the socket" << std::endl;
	shutdown(sockFd, SHUT_RDWR);
	close(sockFd);
	return 0;

TERMINATE:
	// Deallocate resource and shutdown

	return -1;
}
