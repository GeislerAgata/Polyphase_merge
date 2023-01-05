#include "Buffer.h"
#include <fstream>
#include <iostream>
using namespace std;

#define BUFFER_SIZE 512

Buffer::Buffer() {}

Buffer::~Buffer() {
	delete[] buffer;
}

Buffer::Buffer(File* file) {
	buffer = new Pentagon[BUFFER_SIZE];
	position = BUFFER_SIZE;
	end = false;
	counter = 0;
	input_file = file;
	numer_of_read = 0;
}

Pentagon* Buffer::record() {
	if (position == BUFFER_SIZE) {

		numer_of_read++;

		double first, second, third, fourth, fifth;
		Pentagon* new_pentagon;

		ifstream input(input_file->file_name);

		if (!input.good()) {
			cout << "File error (" << input_file->file_name << ") " << endl;
			return nullptr;
		}

		input.seekg(input_file->file_location, ios::beg);
		counter = 0;

		while ((counter < BUFFER_SIZE) && (!end)) {		
			if (input >> first >> second >> third >> fourth >> fifth ) {
				new_pentagon = new Pentagon(first, second, third, fourth, fifth);
				buffer[counter] = *new_pentagon;
				counter++;
				cout << "Sides = { " << first << ", " << second << ", " << third << ", " << fourth << ", " << fifth << " } "<< endl;
			}
			else {
				end = true;
			}
		}
		position = 0;

		input_file->file_location = input.tellg();
		input.close();	
	}
	if (position >= counter && end) {
		return nullptr;
	}
	position++;
	return &(buffer[position - 1]);
}

Pentagon* Buffer::nextRecord() {
	if (position == BUFFER_SIZE) {

		numer_of_read++;

		double first, second, third, fourth, fifth;
		Pentagon* new_pentagon;

		ifstream input(input_file->file_name);
		if (!input.good()) {
			cout << "File error (" << input_file->file_name << ") " << endl;
			return nullptr;
		}

		input.seekg(input_file->file_location, ios::beg);
		counter = 0;

		while ((counter < BUFFER_SIZE) && (!end)) {
			if (input >> first >> second >> third >> fourth >> fifth) {
				new_pentagon = new Pentagon(first, second, third, fourth, fifth);
				buffer[counter] = *new_pentagon;
				counter++;
			}
			else {
				end = true;
			}
		}
		position = 0;

		input_file->file_location = input.tellg();
		input.close();
	}
	if (position >= counter && end) {
		return nullptr;
	}
	position++;
	return &(buffer[position - 1]);
}
