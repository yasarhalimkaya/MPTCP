#include "FileSizeResponse.hpp"
#include <netdb.h>

FileSizeResponse::FileSizeResponse() {
	fileSize = 0;
	fileId = 0;
}

FileSizeResponse::~FileSizeResponse() {

}

uint32_t FileSizeResponse::getFileSize() {
	if (!parse()) {
		return 0;
	}

	return fileSize;
}

uint8_t FileSizeResponse::getFileId() {
	if (!parse()) {
		return 0;
	}

	return fileId;
}

bool FileSizeResponse::parse() {
	// If the response is already parsed, return if it's valid
	if (parsed) {
		return valid;
	}

	// Check if the server returned an error response
	if (buffer[0] == INVALID_REQUEST_TYPE) {
		responseType = INVALID_REQUEST_TYPE;
		goto ERROR;
	}

	if (buffer[0] == INVALID_FILE_ID) {
		responseType = INVALID_FILE_ID;
		goto ERROR;
	}

	// Check if the response type is the expected type
	if (buffer[0] != GET_FILE_SIZE_SUCCESS) {
		goto ERROR;
	}

	fileId = buffer[1];
	fileSize = ntohl(*reinterpret_cast<uint32_t*>(buffer+10));

	parsed = true;
	valid = true;
	return valid;

ERROR:
	parsed = true;
	valid = false;
	return valid;
}
