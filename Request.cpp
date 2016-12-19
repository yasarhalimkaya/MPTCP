#include "Request.hpp"
#include <cstring>

Request::Request() {
	requestTypeNumber = 0;
	memset(data, 0, sizeof(data));
}

uint8_t * Request::getData(){
	return this->data;
}

Request::~Request() {

}
