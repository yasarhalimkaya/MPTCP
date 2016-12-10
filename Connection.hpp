#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "Common.hpp"

#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

// TODO: Add class summary
class Connection {
private:
	bool initialized;
	int sockFd;
	struct hostent * host;
	struct sockaddr_in destAddr;

public:
	Connection(ServerConf serverConf);
	bool send(const unsigned char * message, int size);
	bool recv(unsigned char * message, int size);
	virtual ~Connection();
};

#endif /* CONNECTION_HPP_ */
