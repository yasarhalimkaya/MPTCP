#ifndef FILESIZERESPONSE_HPP_
#define FILESIZERESPONSE_HPP_

#include "Response.hpp"

class FileSizeResponse: public Response {
public:
	FileSizeResponse();

	uint32_t getFileSize();

	virtual ~FileSizeResponse();

private:
	bool parse();
	uint32_t fileSize;
};

#endif /* FILESIZERESPONSE_HPP_ */
