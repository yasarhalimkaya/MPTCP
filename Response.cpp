#include "Response.hpp"

Response::Response() {
	responseType = UNKNOWN;
	parsed = false;
    valid = false;
}

bool Response::isValid() {
	return parse();
}

Response::~Response() {
	
}
