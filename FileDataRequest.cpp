#include "FileDataRequest.hpp"
#include <netdb.h>

FileDataRequest::FileDataRequest(uint8_t fileId, uint32_t startByte, uint32_t endByte) {
	requestType = 3;
	data[0] = requestType;
	data[1] = fileId;
	*reinterpret_cast<uint32_t*>(data+2) = htonl(startByte);
	*reinterpret_cast<uint32_t*>(data+2+sizeof(uint32_t)) = htonl(endByte);
}

FileDataRequest::~FileDataRequest() {

}
