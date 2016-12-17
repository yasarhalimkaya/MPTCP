#include "Request.hpp"

Request::Request() {
	requestTypeNumber = 0;
	memset(data, 0, sizeof(data));
}

unsigned char * Request::getData(){
	return this->data;
}

Request::~Request() {

}
