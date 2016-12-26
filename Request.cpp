#include "Request.hpp"
#include <cstring>

Request::Request() {
	requestType = 0;
	memset(data, 0, sizeof(data));
}

Request::~Request() {

}
