#include "Connection.hpp"

Connection::Connection(ServerConf serverConf) {
	// Create inet structure for given properties
	host = gethostbyname(serverConf.ipAddr.c_str());

	destAddr.sin_family = AF_INET;
	destAddr.sin_port = serverConf.port;
	bcopy(host->h_addr, (char*)&destAddr.sin_addr, host->h_length);

	// Create a UDP socket
	this->sockFd = socket(AF_INET, SOCK_DGRAM, 0);

	if (!sockFd) {
		std::cout << "Connection: Failed to create a socket" << std::endl;
		this->initialized = false;
	} else {
		this->initialized = true;
	}
}

bool Connection::send(const char * message) {
	if (!this->initialized) {
		std::cout << "Connection: Connection is not initialized" << std::endl;
		return false;
	}

	int n = sendto(this->sockFd, message, strlen(message), 0, (struct sockaddr *)&destAddr, sizeof(destAddr));

	if (n == -1) {
		std::cout << "Connection: sendto failed" << std::endl;
		return false;
	}

	if (n < strlen(message)) {
		std::cout << "Connection: Message could not be sent successfully" << std::endl;
		return false;
	}

	return true;
}

Connection::~Connection() {
	if (this->initialized) {
		std::cout << "Shutting down the socket" << std::endl;
		shutdown(this->sockFd, SHUT_RDWR);
		close(this->sockFd);
	}
}
