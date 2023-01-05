#pragma once

#include "Pentagon.h"
#include "File.h"

class Buffer {
private:
	Pentagon* buffer;
	unsigned int counter;
	unsigned int position;
	bool end;
	File* input_file;

public:
	unsigned long long numer_of_read;

	Buffer();
	Buffer(File* file);
	Pentagon* record();
	Pentagon* nextRecord();
	~Buffer();
};

