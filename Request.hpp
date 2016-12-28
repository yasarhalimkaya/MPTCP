#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <iostream>

// TODO: Add class summary
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
