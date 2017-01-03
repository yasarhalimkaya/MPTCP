#ifndef RESPONSE_HPP_
#define RESPONSE_HPP_

#include <iostream>

/**
 * Base class for Response objects.
 * Buffer size is variable, so derived classes
 * should allocate and fill it accordingly.
 */
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

    enum ResponseType {
    	GET_FILE_LIST_SUCCESS = 1,
    	GET_FILE_SIZE_SUCCESS = 2,
    	GET_FILE_DATA_SUCCESS = 3,
		INVALID_REQUEST_TYPE  = 100,
		INVALID_FILE_ID       = 101,
		INVALID_START_OR_END_BYTE = 102,
		UNKNOWN
    };

    inline ResponseType getResponseType() {
    	return responseType;
    }

    bool isValid();

    virtual ~Response();

protected:
    Response();
    virtual bool parse() = 0;

    bool parsed;
    bool valid;
    ResponseType responseType;
    uint8_t* buffer;
    size_t size;
};

#endif /* RESPONSE_HPP_ */
