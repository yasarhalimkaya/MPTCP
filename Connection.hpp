#include "Common.hpp"

#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

class Connection {
private:
	bool initialized;
	int sockFd;
	struct hostent * host;
	struct sockaddr_in destAddr;

public:
	Connection(ServerConf serverConf);
	bool send(const char * message);
	virtual ~Connection();
};
