﻿//Copyright[2019][tkfmhmnk]

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

CsvStream::CsvStream(const char* filename, ios_base::openmode mode = ios_base::in | ios_base::out)
	: fstream(filename, mode)
{
	buf = new char[bufsize];
	if (buf == NULL) {
		cout << "memory allocation failure" << endl;
		exit(1);
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
		if (rdstate() != goodbit) {
			*des = '\0';
			clear();
			return Ret::END_OF_CSV;
		}
		else if (temp == ',') {
			*des = '\0';
			return Ret::NO_ERR;
		}
		else if (temp == '\n') {
#ifdef CR_IS_EXIST
			*(des - 1) = '\0';
#else
			*des = '\0';
#endif
			return Ret::END_OF_ROW;
		}
		*(des++) = temp;
	}
	*des = '\0';
	cout << "The buffer is too small." << endl;
	return Ret::ERR;
}

//現在の位置のセルを整数で読み込む
Ret CsvStream::GetCell(int* des) {
	Ret ret;
	ret = GetCell(buf, bufsize);
	switch (ret) {
	case Ret::NO_ERR:
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