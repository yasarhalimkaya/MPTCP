#include "FileSizeRequest.hpp"

FileSizeRequest::FileSizeRequest(uint8_t fileId) {
	requestTypeNumber = 2;
	data[0] = requestTypeNumber;
	data[1] = fileId;
}

FileSizeRequest::~FileSizeRequest() {

}
