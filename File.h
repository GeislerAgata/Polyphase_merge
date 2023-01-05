#pragma once

class File {
public:
	char* file_name;
	unsigned int file_location;
	File();
	File(char *name, unsigned int location);
	void removeFile();
	~File();
};
