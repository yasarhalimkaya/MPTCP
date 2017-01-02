#ifndef FILELISTRESPONSE_HPP_
#define FILELISTRESPONSE_HPP_

#include "Response.hpp"
#include <vector>
#include <string>

class FileListResponse: public Response {
public:
	FileListResponse();

	uint8_t getNumberOfFiles();
	uint8_t getFileId(uint8_t index);
	std::string getFileName(uint8_t index);

	virtual ~FileListResponse();

private:
	bool parse();

	typedef struct _File {
		uint8_t fileId;
		std::string fileName;
	} File;

	std::vector<File> files;
};

#endif /* FILELISTRESPONSE_HPP_ */
