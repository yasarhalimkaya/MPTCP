#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

class Connection {
private:
	bool initialized;
	char * destIp;
	int destPort;
	int sockFd;
	struct hostent * host;
	struct sockaddr_in destAddr;

public:
	Connection(const char * destIp, int destPort);
	bool send(const char * message);
	virtual ~Connection();
};
