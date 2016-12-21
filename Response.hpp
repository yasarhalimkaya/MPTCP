#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <iostream>

// TODO: Add class summary
class Response
{
public:
    // Buffer to received data is allocated by derived classes
    virtual uint8_t* getBuffer() = 0;
    // Size of the buffer is calculated according to the request
    size_t getSize();
    virtual ~Response();

protected:
    Response();
    uint8_t* buffer;
    size_t size;
};

inline uint8_t * Response::getBuffer(){
    return this->buffer;
}

inline size_t Response::getSize(){
    return this->size;
}

#endif /* RESPONSE_HPP_ */
