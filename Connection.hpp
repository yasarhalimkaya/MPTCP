#ifndef CONNECTION_HPP_
#define CONNECTION_HPP_

#include "Common.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "DeltaTimer.hpp"

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

	enum RECV_STATUS {
		RECV_SUCCESS,
		RECV_TIMEOUT,
		RECV_FAIL
	};

	RECV_STATUS recv(uint8_t * message, size_t size);
	RECV_STATUS recv(Response & response);

	// Receive socket timeout in microseconds, no timeout by default
	uint32_t getTimeout();
	bool setTimeout(uint32_t usecs);

	// Shutting down the sockets inside destructor
	// prevents making copies of the object
	// so, teardown must explicitly be called instead
	void teardown();

	virtual ~Connection();

private:
	bool initialized;
	int sockFd;
	struct hostent * host;
	struct sockaddr_in destAddr;
	uint32_t recvTimeout;
	DeltaTimer timer;
	uint32_t timerCount;
};

#endif /* CONNECTION_HPP_ */
