#ifndef FILESIZEREQUEST_HPP_
#define FILESIZEREQUEST_HPP_

#include "Request.hpp"

class FileSizeRequest: public Request {
public:
	FileSizeRequest(uint8_t fileId);
	virtual ~FileSizeRequest();
};

#endif /* FILESIZEREQUEST_HPP_ */
