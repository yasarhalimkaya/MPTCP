#include "FileSizeRequest.hpp"

FileSizeRequest::FileSizeRequest(uint8_t fileId) {
	requestType = 2;
	data[0] = requestType;
	data[1] = fileId;
}

FileSizeRequest::~FileSizeRequest() {

}
