#ifndef FILEDATAREQUEST_HPP_
#define FILEDATAREQUEST_HPP_

#include "Request.hpp"

class FileDataRequest: public Request {
public:
	FileDataRequest(uint8_t fileId, uint32_t startByte, uint32_t endByte);
	virtual ~FileDataRequest();
};

#endif /* FILEDATAREQUEST_HPP_ */
