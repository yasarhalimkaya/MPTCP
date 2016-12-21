#include "Request.hpp"
#include <cstring>

Request::Request() {
	requestTypeNumber = 0;
	memset(data, 0, sizeof(data));
}

Request::~Request() {

}
