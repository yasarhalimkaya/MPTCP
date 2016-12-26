#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <iostream>

// TODO: Add class summary
class Response {
public:
    // Buffer to received data is allocated by derived classes
    uint8_t* getBuffer();
    // Size of the buffer is calculated according to the request
    size_t getSize();
    virtual ~Response();

protected:
    Response();
    uint8_t responseType;
    uint8_t* buffer;
    size_t size;
};

inline uint8_t * Response::getBuffer() {
    return this->buffer;
}

inline size_t Response::getSize() {
    return this->size;
}

#endif /* RESPONSE_HPP_ */
