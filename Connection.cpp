#include "Connection.hpp"
#include <cstring>

Connection::Connection(ServerConf serverConf) :
	recvTimeout(0),
	timerCount(0)
{
	// Create inet structure to send data to server
	host = gethostbyname(serverConf.ipAddr.c_str());

	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(serverConf.port);
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
		this->initialized = false;
	}

	if (!sockFd) {
		this->initialized = false;
	} else {
		this->initialized = true;
	}
}

bool Connection::send(const uint8_t * message, size_t size) {
	if (!this->initialized) {
		return false;
	}

	int n = sendto(this->sockFd, message, size, 0, (struct sockaddr *)&destAddr, sizeof(destAddr));

	if (n == -1) {
		return false;
	}

	if (n < size) {
		return false;
	}

	return true;
}

bool Connection::send(Request & request){
	return send(request.getData(), request.getSize());
}

Connection::RECV_STATUS Connection::recv(uint8_t * message, size_t size) {
	if (!this->initialized) {
		return RECV_FAIL;
	}

	if (message == 0) {
		return RECV_FAIL;
	}

	sockaddr_in from;
	socklen_t fromLen = sizeof(from);
	timer.start();
	int n = recvfrom(this->sockFd, message, size, 0, (struct sockaddr *)&from, &fromLen);
	timer.stop();
	recvTimeout = (recvTimeout + timer.getDeltaUSec()) / ++timerCount;
	//std::cout << "recvTimeout : " << recvTimeout  << " timerCount " << timerCount << std::endl;

	if (n == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return RECV_TIMEOUT;
		} else {
			return RECV_FAIL;
		}
	}

	if (n <= 0) { // No data received
		return RECV_FAIL;
	}

	return RECV_SUCCESS;
}

Connection::RECV_STATUS Connection::recv(Response & response) {
	return recv(response.getBuffer(), response.getSize());
}

bool Connection::setTimeout(uint32_t usecs) {
	recvTimeout = usecs;

	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = recvTimeout;

	if (setsockopt(this->sockFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		return false;
	}

	return true;
}

uint32_t Connection::getTimeout() {
	return recvTimeout;
}

void Connection::teardown() {
	if (this->initialized) {
		shutdown(this->sockFd, SHUT_RDWR);
		close(this->sockFd);
	}
}

Connection::~Connection() {

}
