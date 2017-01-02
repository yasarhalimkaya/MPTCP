#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "Common.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * Connection object for UDP transmissions
 * Given a ServerConf containing server ip and port number
 * it wraps the c style inet structures and sendto and recvfrom
 * functions.
 */
class Connection {
public:
	Connection(ServerConf serverConf);

	bool send(const uint8_t * message, size_t size);
	bool send(Request & request);

	bool recv(uint8_t * message, size_t size);
	bool recv(Response & response);

	virtual ~Connection();

private:
	bool initialized;
	int sockFd;
	struct hostent * host;
	struct sockaddr_in destAddr;
};

#endif /* CONNECTION_HPP_ */
