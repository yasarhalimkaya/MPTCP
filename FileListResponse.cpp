#include "FileListResponse.hpp"

FileListResponse::FileListResponse() {
	size = 1500;
	buffer = new uint8_t[this->size];
}

FileListResponse::~FileListResponse() {

}

uint8_t FileListResponse::getNumberOfFiles() {
	if (!parse()) {
		return -1;
	}

	return files.size();
}

uint8_t FileListResponse::getFileId(uint8_t index) {
	if (!parse() || index >= files.size()) {
		return -1;
	}

	return files.at(index).fileId;
}

std::string FileListResponse::getFileName(uint8_t index) {
	if (!parse() || index >= files.size()) {
		return std::string("");
	}

	return files.at(index).fileName;
}

bool FileListResponse::parse() {
	// If the response is already parsed, return if it's valid
	if (parsed) {
		std::cout << "FileListResponse: already parsed valid : " << valid << std::endl;
		return valid;
	}

	// Check if the server returned an error response
	if (buffer[0] == INVALID_REQUEST_TYPE) {
		std::cout << "FileListResponse: INVALID_REQUEST_TYPE" << std::endl;
		responseType = INVALID_REQUEST_TYPE;
		goto ERROR;
	}

	// Check if the response type is the expected type
	if (buffer[0] != GET_FILE_LIST_SUCCESS) {
		std::cout << "FileListResponse: response type is not GET_FILE_LIST_SUCCESS" << std::endl;
		goto ERROR;
	}

	// Parse the response
	for (int i = 0, index = 10; i < (int)buffer[1]; i++) {
		File file;

		file.fileId = buffer[index];
		index++;

		file.fileName.append((const char *)(buffer+index));
		index += file.fileName.size();

		// For the \0 at the end
		index++;

		files.push_back(file);
	}

	valid = true;
	return valid;

ERROR:
	parsed = true;
	valid = false;
	return valid;
}





