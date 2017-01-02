#include "FileSizeResponse.hpp"

FileSizeResponse::FileSizeResponse() {
	size = 14;
	buffer = new uint8_t[size];
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
		std::cout << "FileSizeResponse: already parsed valid : " << valid << std::endl;
		return valid;
	}

	// Check if the server returned an error response
	if (buffer[0] == INVALID_REQUEST_TYPE) {
		std::cout << "FileSizeResponse: INVALID_REQUEST_TYPE" << std::endl;
		responseType = INVALID_REQUEST_TYPE;
		goto ERROR;
	}

	if (buffer[0] == INVALID_FILE_ID) {
		std::cout << "FileSizeResponse: INVALID_FILE_ID" << std::endl;
		responseType = INVALID_FILE_ID;
		goto ERROR;
	}

	// Check if the response type is the expected type
	if (buffer[0] != GET_FILE_SIZE_SUCCESS) {
		std::cout << "FileSizeResponse: response type is not GET_FILE_SIZE_SUCCESS" << std::endl;
		goto ERROR;
	}

	fileId = buffer[1];
	fileSize = ntohl(*reinterpret_cast<uint32_t*>(buffer+10));

	valid = true;
	return valid;

ERROR:
	parsed = true;
	valid = false;
	return valid;
}
