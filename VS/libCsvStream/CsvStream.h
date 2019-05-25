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

#ifndef CSVSTREAM_H
#define CSVSTREAM_H

#include<ios>
#include<fstream>
#include<string>

namespace CsvStreamNS {
	enum class Ret : int {
		OK = 1,				//!<処理に成功した。
		END_OF_ROW,			//!<処理に成功し、その結果、行の最後に達した
		BEGIN_OF_ROW,		//!<処理に成功し、その結果、行の最初に達した
		END_OF_CSV,			//!<処理に成功し、その結果、ファイルのEOFに達した
		BEGIN_OF_CSV,		//!<処理に成功し、その結果、ファイルの先頭に達した
		ERR,				//!<処理に失敗した
	};

	class CsvStream : public std::fstream {
	public:
		CsvStream() {}
		CsvStream(
			const char* filename,
			std::ios_base::openmode mode = ios_base::in | ios_base::out ,
			bool existCR = true,
			std::ostream* errorOutputStream = nullptr);
		~CsvStream();
		
		/**
		ファイルの現在の入力位置のセルの文字列を読み取る
		*/
		Ret readCell(char* des, std::streamsize n);
		
		/**
		ファイルの現在の入力位置のセルの文字列を読み取る
		*/
		Ret readCell(std::string& des);

		/**
		ファイルの現在の入力位置のセルの整数を読み取る
		*/
		Ret readCell(int* des);

		/**
		行中の次のセルの先頭の文字までファイルの入力位置をシークする
		@return 次のセルがない場合ERRを返す
		*/
		Ret seekToNextCell();

		/**
		次の行の先頭の文字までファイルの入力位置をシークする
		@return 次の行がない場合ERRを返す
		*/
		Ret seekToNextCol();

		/**
		行中の前のセルの先頭の文字までファイルの入力位置をシークする
		@return 前のセルがない場合ERRを返す
		*/
		Ret seekToPrevCell();

		/**
		前の行の先頭の文字までファイルの入力位置をシークする
		@return 前の行がない場合ERRを返す
		*/
		Ret seekToPrevCol();

		/**
		現在のセルの先頭の文字までファイルの入力位置をシークする
		*/
		Ret seekToCurrCol();

	private:
		char* buf;
		static const int bufsize;
		std::ostream* errOutputStream;
		bool CR;
	};

}


#endif /* CSVSTREAM_H */

