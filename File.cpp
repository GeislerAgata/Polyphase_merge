#include "File.h"

#include <stdio.h>

File::File() {};

File::File(char* name, unsigned int location) {
	file_name = name;
	file_location = location;
}

void File::removeFile() {
	remove(file_name);
}

File::~File() {
	remove(file_name);
}

