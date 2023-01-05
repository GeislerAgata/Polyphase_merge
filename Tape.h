#pragma once

#include "Pentagon.h"
#include "File.h"

class Tape {
private:
	Pentagon* buffer;
	unsigned int position;	
	File* output_file;
	bool print;

public:
	unsigned long long numer_of_write;
	unsigned int series;
	unsigned int dummies;

	Tape();
	Tape(File* file);
	Tape(File* file, bool display);
	bool saveRecord(Pentagon* record);
	bool saveValue(Pentagon* record);
	void save();
	~Tape();
};

