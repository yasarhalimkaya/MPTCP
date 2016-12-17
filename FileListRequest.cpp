#include "FileListRequest.hpp"

FileListRequest::FileListRequest() {
	requestTypeNumber = 1;
	data[0] = requestTypeNumber;
}

FileListRequest::~FileListRequest() {

}
