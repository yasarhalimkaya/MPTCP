#ifndef REQUEST_HPP_
#define REQUEST_HPP_

#include <iostream>

// TODO: Add class summary
class Request {
public:
	unsigned char * getData();
	int getSize();
	virtual ~Request();

protected:
	Request(); // Instantiation is not allowed

	unsigned char requestTypeNumber;
	const static int REQUEST_SIZE = 10;
	unsigned char data[REQUEST_SIZE];
};

inline int Request::getSize(){
	return REQUEST_SIZE;
}

#endif /* REQUEST_HPP_ */
