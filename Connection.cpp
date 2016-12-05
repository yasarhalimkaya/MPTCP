#include "Connection.hpp"

Connection::Connection(const char * destIp, int destPort) {
	this->destPort = destPort;

	this->destIp = new char(sizeof(destIp));

	if (this->destIp) {
		strcpy(this->destIp, destIp);
	}

	// Create inet structure for given properties
	host = gethostbyname(this->destIp);

	std::cout << "Given host addr : " << this->destIp << std::endl;
	std::cout << "Host addr : " << host->h_name << std::endl;
	std::cout << "Port : " << this->destPort << std::endl;

	destAddr.sin_family = AF_INET;
	destAddr.sin_port = this->destPort;
	bcopy(host->h_addr, (char*)&destAddr.sin_addr, host->h_length);

	// Create a UDP socket for localhost
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
	if (this->destIp) {
		delete this->destIp;
	}

	if (this->initialized) {
		std::cout << "Shutting down the socket" << std::endl;
		shutdown(this->sockFd, SHUT_RDWR);
		close(this->sockFd);
	}
}
