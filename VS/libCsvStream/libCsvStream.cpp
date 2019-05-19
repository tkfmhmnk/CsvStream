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

//現在の位置のセルを文字列で読み込む
Ret CsvStream::GetCell(char* des, streamsize n) {
	char temp;
	while ((n--) > 1) {
		get(temp);
		if (eof()) {
			*des = '\0';
			//clear();					//ライブラリを呼び出す側でclearする
			return Ret::END_OF_CSV;
		}
		else if (rdstate() != goodbit) {
			*des = '\0';
			//clear();					//ライブラリを呼び出す側でclearする
			return Ret::ERR;
		}
		else if (temp == ',') {
			*des = '\0';
			return Ret::OK;
		}
		else if (temp == '\n') {
			if (CR) {
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

//現在の位置のセルを整数で読み込む
Ret CsvStream::GetCell(int* des) {
	Ret ret;
	ret = GetCell(buf, bufsize);
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