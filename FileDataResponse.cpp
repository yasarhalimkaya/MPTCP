#include "FileDataResponse.hpp"

FileDataResponse::FileDataResponse() {
	fileId = 0;
	startByte = 0;
	endByte = 0;
	size = MAX_DATA_SIZE+HEADER_SIZE;
	buffer = new uint8_t[size];
}

FileDataResponse::FileDataResponse(const FileDataResponse & obj) {
	fileId = obj.fileId;
	startByte = obj.startByte;
	endByte = obj.endByte;
	size = obj.size;
	parsed = obj.parsed;
	valid = obj.valid;
	buffer = new uint8_t[size];
	memcpy(buffer, obj.buffer, size);
}

FileDataResponse::~FileDataResponse() {

}

uint8_t FileDataResponse::getFileId() {
	if (!parse()) {
		return 0;
	}

	return fileId;
}

uint32_t FileDataResponse::getStartByte() {
	if (!parse()) {
		return 0;
	}

	return startByte;
}

uint32_t FileDataResponse::getEndByte() {
	if (!parse()) {
		return 0;
	}

	return endByte;
}

uint8_t* FileDataResponse::getData() {
	return buffer + HEADER_SIZE;
}

uint32_t FileDataResponse::getDataSize() {
	return getEndByte() - getStartByte() + 1;
}

bool FileDataResponse::parse() {
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

	if (buffer[0] == INVALID_START_OR_END_BYTE) {
		responseType = INVALID_START_OR_END_BYTE;
		goto ERROR;
	}

	// Check if the response type is the expected type
	if (buffer[0] != GET_FILE_DATA_SUCCESS) {
		goto ERROR;
	}

	fileId = buffer[1];
	startByte = ntohl(*reinterpret_cast<uint32_t*>(buffer+2));
	endByte = ntohl(*reinterpret_cast<uint32_t*>(buffer+2+sizeof(uint32_t)));

	parsed = true;
	valid = true;
	return valid;

ERROR:
	parsed = true;
	valid = false;
	return valid;
}
