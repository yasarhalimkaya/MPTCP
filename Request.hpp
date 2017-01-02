#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <iostream>

/**
 * Base class for Request objects.
 * As the request packet size is constant,
 * it allocates the data buffer in the stack
 * Derived classes should only fill the buffer accordingly.
 */
class Request {
public:
	inline uint8_t * getData() {
		return data;
	}

	inline size_t getSize() {
		return REQUEST_SIZE;
	}

	virtual ~Request();

protected:
	Request();
	uint8_t requestType;
	const static int REQUEST_SIZE = 10;
	uint8_t data[REQUEST_SIZE];
};

#endif /* REQUEST_HPP_ */
