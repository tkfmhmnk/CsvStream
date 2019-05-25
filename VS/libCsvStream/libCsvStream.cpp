//Copyright[2019][tkfmhmnk]

//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at

//http ://www.apache.org/licenses/LICENSE-2.0

//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include "stdafx.h"

#include"CsvStream.h"
#include<iostream>
#include <limits>

using namespace std;
using namespace CsvStreamNS;

const int CsvStream::bufsize = 1024;

CsvStream::CsvStream(const char* filename, ios_base::openmode mode, bool existCR, std::ostream* errorOutputStream)
	: fstream(filename, mode)
{
	buf = new char[bufsize];
	errOutputStream = errorOutputStream;
	CR = existCR;

	if (is_open() == false) {
		if (errOutputStream != nullptr) *errOutputStream << "Failed to open file : filename" << endl;
	}

	if (buf == NULL) {
		if(errOutputStream != nullptr) *errOutputStream << "Failed to allocate memory" << endl;
		close();
	}
}

CsvStream::~CsvStream() {
	delete[] buf;
}

Ret CsvStream::readCell(char* des, streamsize n) {
	char temp;
	if (seekToCurrCol() == Ret::ERR) return Ret::ERR;
	while ((n--) > 1) {
		get(temp);
		if (eof()) {
			*des = '\0';
			return Ret::END_OF_CSV;
		}
		else if (rdstate() != goodbit) {
			*des = '\0';
			return Ret::ERR;
		}
		else if (temp == ',') {
			*des = '\0';
			return Ret::OK;
		}
		else if (temp == '\n') {
			if (*(des - 1)== '\r') {
				*(des - 1) = '\0';
			}
			else {
				*des = '\0';
			}
			return Ret::END_OF_ROW;
		}
		*(des++) = temp;
	}
	*des = '\0';
	if (errOutputStream != nullptr) *errOutputStream << "The buffer is too small." << endl;
	return Ret::ERR;
}

Ret CsvStream::readCell(std::string& des){
	char temp;
	des.clear();
	if (seekToCurrCol() == Ret::ERR) return Ret::ERR;
	while (1) {
		get(temp);
		if (eof()) {
			return Ret::END_OF_CSV;
		}
		else if (rdstate() != goodbit) {
			return Ret::ERR;
		}
		else if (temp == ',') {
			return Ret::OK;
		}
		else if (temp == '\n') {
			if (des.back() == '\r') {
				des.pop_back();
			}
			return Ret::END_OF_ROW;
		}
		des.push_back(temp);
	}
	return Ret::ERR;
}

Ret CsvStream::readCell(int* des) {
	Ret ret;
	ret = readCell(buf, bufsize);
	switch (ret) {
	case Ret::OK:
	case Ret::END_OF_ROW:
	case Ret::END_OF_CSV:
		*des = atoi(buf);
		return ret;
		break;
	case Ret::ERR:
	default:
		*des = 0;
		return ret;
		break;
	}
}

Ret CsvStream::seekToNextCell() {
	char temp;
	while (1) {
		get(temp);
		if (eof()) {
			return Ret::END_OF_CSV;
		}
		else if (rdstate() != goodbit) {
			return Ret::ERR;
		}
		else if (temp == ',') {
			return Ret::OK;
		}
		else if (temp == '\n') {
			return Ret::END_OF_ROW;
		}
	}
	return Ret::ERR;
}

Ret CsvStream::seekToNextCol(){
	char temp;
	while (1) {
		ignore(numeric_limits<streamsize>::max(), '\n');
		if (eof()) {
			return Ret::END_OF_CSV;
		}
		else if (rdstate() != goodbit) {
			return Ret::ERR;
		}
		else {
			return Ret::OK;
		}
	}
	return Ret::ERR;
}

Ret CsvStream::seekToPrevCell() {
	char temp;
	do {
		if (rdstate() != goodbit) return Ret::ERR;

		if (tellg() == 0) return Ret::ERR;
		seekg(-1, ios_base::cur);
		temp = peek();
		if (temp == '\n') {
			get(temp);
			return Ret::ERR;
		}
	} while (temp != ',');

	do {
		if (rdstate() != goodbit) return Ret::ERR;

		if (tellg() == 0) return Ret::BEGIN_OF_CSV;
		seekg(-1, ios_base::cur);
		temp = peek();
		if (temp == '\n') {
			get(temp);
			return Ret::BEGIN_OF_ROW;
		}
	} while (temp != ',');

	get(temp);
	return Ret::OK;
}

Ret CsvStream::seekToPrevCol() {
	char temp;
	do {
		if (rdstate() != goodbit) return Ret::ERR;

		if (tellg() == 0) return Ret::ERR;
		seekg(-1, ios_base::cur);
		temp = peek();
	} while (temp != '\n');

	do {
		if (rdstate() != goodbit) return Ret::ERR;

		if (tellg() == 0) return Ret::BEGIN_OF_CSV;
		seekg(-1, ios_base::cur);
		temp = peek();
	} while (temp != '\n');

	get(temp);
	return Ret::OK;
}

Ret CsvStream::seekToCurrCol() {
	char temp;
	do {
		if (rdstate() != goodbit) return Ret::ERR;

		if (tellg() == 0) return Ret::BEGIN_OF_CSV;
		seekg(-1, ios_base::cur);
		temp = peek();
		if (temp == '\n') {
			get(temp);
			return Ret::BEGIN_OF_ROW;
		}
	} while (temp != ',');

	get(temp);
	return Ret::OK;
}