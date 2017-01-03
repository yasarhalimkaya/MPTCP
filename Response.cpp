#include "Response.hpp"

Response::Response() {
	responseType = UNKNOWN;
	buffer = 0;
    size = 0;
    parsed = false;
    valid = false;
}

bool Response::isValid() {
	return parse();
}

Response::~Response() {
	if (buffer) {
		delete[] buffer;
		buffer = 0;
	}
}
