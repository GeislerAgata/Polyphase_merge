#include "Tape.h"
#include <fstream>
#include <iostream>
using namespace std;

#define TAPE_SIZE 512

Tape::Tape() {};

Tape::Tape(File* file) {
	series = 0;

	buffer = new Pentagon[TAPE_SIZE];
	position = 0;
	output_file = file;
	output_file->removeFile();
	print = false;
	numer_of_write = 0;
};

Tape::Tape(File* file, bool display) {
	series = 0;

	buffer = new Pentagon[TAPE_SIZE];
	position = 0;
	output_file = file;
	output_file->removeFile();
	print = display;
	numer_of_write = 0;
};

bool Tape::saveRecord(Pentagon* record) {
	if (position == TAPE_SIZE) {
		numer_of_write++;
		ofstream output(output_file->file_name, ios::out | ios::app);

		if (print) {
			cout << "File after next phase" << endl;
		}
		for (position = 0; position < TAPE_SIZE; position++) {
			output << buffer[position].side1 << " " << buffer[position].side2 << " " << buffer[position].side3 << " " << buffer[position].side4 << " " << buffer[position].side5 << endl;
			if (print) {
				cout << "Save record sides = { " << buffer[position].side1 << ", " << buffer[position].side2 << ", " << buffer[position].side3 << ", " << buffer[position].side4 << ", " << buffer[position].side5 << " }" << endl;
			}
		}
		position = 0;
		output.close();		
	}
	if (record == nullptr) {
		return false;
	}
	buffer[position] = *record;
	position++;
	return true;
};

bool Tape::saveValue(Pentagon* record) {
	if (position == TAPE_SIZE) {
		numer_of_write++;
		ofstream output(output_file->file_name, ios::out | ios::app);

		for (position = 0; position < TAPE_SIZE; position++) {
			output << buffer[position].side1 << " " << buffer[position].side2 << " " << buffer[position].side3 << " " << buffer[position].side4 << " " << buffer[position].side5 << endl;
			cout << "Sides = { " << buffer[position].side1 << ", " << buffer[position].side2 << ", " << buffer[position].side3 << ", " << buffer[position].side4 << ", " << buffer[position].side5 << " }" << endl;
			
		}
		position = 0;
		output.close();
	}
	if (record == nullptr) {
		return false;
	}
	buffer[position] = *record;
	position++;
	return true;
};

void Tape::save() {
	numer_of_write++;
	ofstream output(output_file->file_name, ios::out | ios::app);

	for (int i = 0; i < position; i ++) {
		output << buffer[i].side1 << " " << buffer[i].side2 << " " << buffer[i].side3 << " " << buffer[i].side4 << " " << buffer[i].side5 << endl;
	}
	position = 0;
	output.close();
};

Tape::~Tape() {
	numer_of_write++;

	ofstream writes("writes.csv", ios::out | ios::app);
	writes << numer_of_write << endl;
	writes.close();

	ofstream output(output_file->file_name, ios::out | ios::app);

	for (int i = 0; i < position; i++) {
		output << buffer[i].side1 << " " << buffer[i].side2 << " " << buffer[i].side3 << " " << buffer[i].side4 << " " << buffer[i].side5 << endl;
		if (print) {
			if (i == 0) {
				cout << endl << "File after phase "  << endl;
			}
			cout << "Sides = { " << buffer[i].side1 << ", " << buffer[i].side2 << ", " << buffer[i].side3 << ", " << buffer[i].side4 << ", " << buffer[i].side5 << " } = " << buffer[i].perimeter << endl;
		}
	}
	output.close();
};