#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <iostream>

// TODO: Add class summary
class Response {
public:
    // Buffer to receive data is allocated by derived classes
    inline uint8_t* getBuffer() {
    	return buffer;
    }

    // Size of the buffer is calculated according to the request
    inline size_t getSize() {
    	return size;
    }

    virtual ~Response();

protected:
    Response();
    uint8_t responseType;
    uint8_t* buffer;
    size_t size;
};

#endif /* RESPONSE_HPP_ */
