#include "Connection.hpp"
#include <cstring>

Connection::Connection(ServerConf serverConf) {
	// Create inet structure to send data to server
	host = gethostbyname(serverConf.ipAddr.c_str());

	destAddr.sin_family = AF_INET;
	destAddr.sin_port = serverConf.port;
	bcopy(host->h_addr, (char*)&destAddr.sin_addr, host->h_length);

	// Create a UDP socket
	this->sockFd = socket(AF_INET, SOCK_DGRAM, 0);

	// Create inet structure for client to receive data
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = 0;
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind socket to client address to be able to receive data
	if (bind(sockFd, (const sockaddr *)&clientAddr, sizeof(clientAddr)) != 0) {
		std::cout << "Failed to bind the socket to given address" << std::endl;
		this->initialized = false;
	}

	if (!sockFd) {
		std::cout << "Connection: Failed to create a socket" << std::endl;
		this->initialized = false;
	} else {
		this->initialized = true;
	}
}

bool Connection::send(const unsigned char * message, int size) {
	if (!this->initialized) {
		std::cout << "Connection: Connection is not initialized" << std::endl;
		return false;
	}

	int n = sendto(this->sockFd, message, size, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));

	if (n == -1) {
		std::cout << "Connection: sendto failed" << std::endl;
		return false;
	}

	if (n < size) {
		std::cout << "Connection: Message could not be sent successfully" << std::endl;
		return false;
	}

	return true;
}

bool Connection::recv(unsigned char * message, int size) {
	if (!this->initialized) {
		std::cout << "Connection: Connection is not initialized" << std::endl;
		return false;
	}

	sockaddr_in from;
	socklen_t fromLen = sizeof(from);
	int n = recvfrom(this->sockFd, message, size, 0, (struct sockaddr *)&from, &fromLen);

	if (n == -1) {
		std::cout << "Connection: Recvfrom failed" << std::endl;
		return false;
	}

	if (n <= 0) {
		std::cout << "Connection: No data received" << std::endl;
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
