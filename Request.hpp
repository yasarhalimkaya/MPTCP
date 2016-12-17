#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <iostream>

// TODO: Add class summary
class Request {
public:
	uint8_t * getData();
	int getSize();
	virtual ~Request();

protected:
	Request(); // Instantiation is not allowed

	uint8_t requestTypeNumber;
	const static int REQUEST_SIZE = 10;
	uint8_t data[REQUEST_SIZE];
};

inline int Request::getSize(){
	return REQUEST_SIZE;
}

#endif /* REQUEST_HPP_ */
