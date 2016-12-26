#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <iostream>

// TODO: Add class summary
class Request {
public:
	uint8_t * getData();
	size_t getSize();
	virtual ~Request();

protected:
	Request();
	uint8_t requestType;
	const static int REQUEST_SIZE = 10;
	uint8_t data[REQUEST_SIZE];
};

inline uint8_t * Request::getData(){
    return this->data;
}

inline size_t Request::getSize(){
	return REQUEST_SIZE;
}

#endif /* REQUEST_HPP_ */
