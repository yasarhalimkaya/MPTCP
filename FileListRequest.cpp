#include "FileListRequest.hpp"

FileListRequest::FileListRequest() {
	requestType = 1;
	data[0] = requestType;
}

FileListRequest::~FileListRequest() {

}
