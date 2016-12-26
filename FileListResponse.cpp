#include "FileListResponse.hpp"

FileListResponse::FileListResponse() {
	this->responseType = 1;
	this->size = 1500;
	this->buffer = new uint8_t[this->size];
}

FileListResponse::~FileListResponse() {
	if (this->buffer) {
		delete[] this->buffer;
		this->buffer = 0;
	}
}
