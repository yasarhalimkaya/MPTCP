#ifndef FILEDATARESPONSE_HPP_
#define FILEDATARESPONSE_HPP_

#include "Response.hpp"

class FileDataResponse: public Response {
public:
	FileDataResponse();
	FileDataResponse(const FileDataResponse &);

	uint8_t getFileId();
	uint32_t getStartByte();
	uint32_t getEndByte();

	uint8_t* getData();
	uint32_t getDataSize();

	virtual ~FileDataResponse();

private:
	bool parse();
	uint8_t fileId;
	uint32_t startByte;
	uint32_t endByte;
};

#endif /* FILEDATARESPONSE_HPP_ */
