#ifndef FILESIZERESPONSE_HPP_
#define FILESIZERESPONSE_HPP_

#include "Response.hpp"

class FileSizeResponse: public Response {
public:
	FileSizeResponse();

	uint8_t getFileId();
	uint32_t getFileSize();

	virtual ~FileSizeResponse();

private:
	bool parse();
	uint8_t fileId;
	uint32_t fileSize;
};

#endif /* FILESIZERESPONSE_HPP_ */
