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

#include<iostream>
#include<fstream>
#include<string>
#include<limits>


/**
	所望の型で特定の文字を取得できる関数の定義を展開する
*/
#define _CSVSTREAM_MULTITYPE_CHAR(name,val) template<class CharT> constexpr CharT Get##name() {\
		return val;\
	};\
	template<> constexpr char Get##name<char>(){\
		return val;\
	};\
	template<> constexpr wchar_t Get##name<wchar_t>(){\
		return L##val;\
	};\
	template<> constexpr char16_t Get##name<char16_t>(){\
		return u##val;\
	};\
	template<> constexpr char32_t Get##name<char32_t>(){\
		return U##val;\
	}

/**
	文字列を複数の文字列リテラルにして展開する
*/
#define _CSVSTREAM_MULTITYPE_STR(arg) {arg , L##arg , u##arg , U##arg} 

namespace CsvStreamNS {
	enum class Ret : int {
		OK = 1,				//!<処理に成功した。
		END_OF_ROW,			//!<処理に成功し、その結果、行の最後に達した
		BEGIN_OF_ROW,		//!<処理に成功し、その結果、行の最初に達した
		END_OF_CSV,			//!<処理に成功し、その結果、ファイルのEOFに達した
		BEGIN_OF_CSV,		//!<処理に成功し、その結果、ファイルの先頭に達した
		ERR,				//!<処理に失敗した
	};

	_CSVSTREAM_MULTITYPE_CHAR(Comma, ',');
	_CSVSTREAM_MULTITYPE_CHAR(NullChar, '\0');
	_CSVSTREAM_MULTITYPE_CHAR(CR, '\r');
	_CSVSTREAM_MULTITYPE_CHAR(LF, '\n');



	template<class CharT> class basic_CsvStream : public std::basic_fstream<CharT>{
	private:
		CharT* buf;
		static constexpr int bufsize = 1024;
		std::basic_ostream<CharT>* errOutputStream;
		bool _CR;

		/**
			メッセージの定義
		*/
		enum class Msg : int {
			FileOpenError = 0,			//!<ファイルオープンエラー
			MemoryAllocationError,		//!<メモリ確保などのエラー
			BufferSizeError,			//!<バッファのサイズが足りない
			Num							//!<メッセージの数を取得するための列挙定数
		};

		/**
			複数の文字列型をまとめた構造体
		*/
		struct MultiTypeMessage {
			const char* c;
			const wchar_t* L;
			const char16_t* u;
			const char32_t* U;
		};

		/**
			エラーなどのメッセージ配列
		*/
		static constexpr MultiTypeMessage message[(int)Msg::Num] = {
			_CSVSTREAM_MULTITYPE_STR("Failed to open file : filename"),
			_CSVSTREAM_MULTITYPE_STR("Failed to allocate memory"),
			_CSVSTREAM_MULTITYPE_STR("The buffer is too small.")
		};

		/**
			messageからメッセージを取得する
			@param msg 取得したいメッセージ
		*/
		template<class CharTT> static const CharTT* getMessage(Msg msg) {
			return message[(int)msg].c;
		};

		/**
			char型で特殊化したgetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const char* getMessage<char>(Msg msg) {
			return message[(int)msg].c;
		};

		/**
			wchar_t型で特殊化したgetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const wchar_t* getMessage<wchar_t>(Msg msg) {
			return message[(int)msg].L;
		};
		
		/**
			char16_t型で特殊化したgetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const char16_t* getMessage<char16_t>(Msg msg) {
			return message[(int)msg].u;
		};

		/**
			char32_t型で特殊化したgetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const char32_t* getMessage<char32_t>(Msg msg) {
			return message[(int)msg].U;
		};

		static constexpr CharT comma = GetComma<CharT>();			//!<コンマ
		static constexpr CharT nullChar = GetNullChar<CharT>();		//!<null文字
		static constexpr CharT CR = GetCR<CharT>();					//!<キャリッジリターン
		static constexpr CharT LF = GetLF<CharT>();					//!<ラインフィード

	public:
		basic_CsvStream() {
		};

		/**
			@param filename 開きたいファイルのパス
			@param mode ファイルを開く時のモード
			@param existCR CRの有無
			@param errorOutputStream エラーなどのメッセージの出力先。不要ならnullptrで良い
		*/
		basic_CsvStream(
			const CharT* filename,
			std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out ,
			bool existCR = true,
			std::basic_ostream<CharT>* errorOutputStream = nullptr) : std::basic_fstream<CharT>(filename, mode)
		{
			buf = new CharT[bufsize];
			errOutputStream = errorOutputStream;
			_CR = existCR;

			if (std::basic_fstream<CharT>::is_open() == false) {
				if (errOutputStream != nullptr) *errOutputStream << getMessage<CharT>(Msg::FileOpenError) << filename << std::endl;
			}

			if (buf == NULL) {
				if (errOutputStream != nullptr) *errOutputStream << getMessage<CharT>(Msg::MemoryAllocationError) << std::endl;
				std::basic_fstream<CharT>::close();
			}
		};

		~basic_CsvStream() {
			delete[] buf;
		};
		
		/**
		ファイルの現在の入力位置のセルの文字列を読み取る
		*/
		Ret readCell(CharT* des, std::streamsize n) {
			CharT temp;
			if (seekToCurrCol() == Ret::ERR) return Ret::ERR;
			while ((n--) > 1) {
				std::basic_fstream<CharT>::get(temp);
				if (std::basic_fstream<CharT>::eof()) {
					*des = nullChar;
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) {
					*des = nullChar;
					return Ret::ERR;
				}
				else if (temp == comma) {
					*des = nullChar;
					return Ret::OK;
				}
				else if (temp == LF) {
					if (*(des - 1) == CR) {
						*(des - 1) = nullChar;
					}
					else {
						*des = nullChar;
					}
					return Ret::END_OF_ROW;
				}
				*(des++) = temp;
			}
			*des = nullChar;
			if (errOutputStream != nullptr) *errOutputStream << getMessage<CharT>(Msg::BufferSizeError) << std::endl;
			return Ret::ERR;
		};
		
		/**
		ファイルの現在の入力位置のセルの文字列を読み取る
		*/
		Ret readCell(std::basic_string<CharT>& des) {
			CharT temp;
			des.clear();
			if (seekToCurrCol() == Ret::ERR) return Ret::ERR;
			while (1) {
				std::basic_fstream<CharT>::get(temp);
				if (std::basic_fstream<CharT>::eof()) {
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) {
					return Ret::ERR;
				}
				else if (temp == comma) {
					return Ret::OK;
				}
				else if (temp == LF) {
					if (des.back() == CR) {
						des.pop_back();
					}
					return Ret::END_OF_ROW;
				}
				des.push_back(temp);
			}
			return Ret::ERR;
		};

		/**
		ファイルの現在の入力位置のセルの整数を読み取る
		*/
		Ret readCell(int* des) {
			Ret ret;
			ret = readCell(buf, bufsize);
			switch (ret) {
			case Ret::OK:
			case Ret::END_OF_ROW:
			case Ret::END_OF_CSV:
				//*des = atoi(buf);	//TODO stoi
				return ret;
				break;
			case Ret::ERR:
			default:
				*des = 0;
				return ret;
				break;
			}
		};

		/**
		行中の次のセルの先頭の文字までファイルの入力位置をシークする
		@return 次のセルがない場合ERRを返す
		*/
		Ret seekToNextCell() {
			CharT temp;
			while (1) {
				std::basic_fstream<CharT>::get(temp);
				if (std::basic_fstream<CharT>::eof()) {
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) {
					return Ret::ERR;
				}
				else if (temp == comma) {
					return Ret::OK;
				}
				else if (temp == LF) {
					return Ret::END_OF_ROW;
				}
			}
			return Ret::ERR;
		};

		/**
		次の行の先頭の文字までファイルの入力位置をシークする
		@return 次の行がない場合ERRを返す
		*/
		Ret seekToNextCol() {
			CharT temp;
			while (1) {
				std::basic_fstream<CharT>::ignore(numeric_limits<std::streamsize>::max(), LF);
				if (std::basic_fstream<CharT>::eof()) {
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) {
					return Ret::ERR;
				}
				else {
					return Ret::OK;
				}
			}
			return Ret::ERR;
		};

		/**
		行中の前のセルの先頭の文字までファイルの入力位置をシークする
		@return 前のセルがない場合ERRを返す
		*/
		Ret seekToPrevCell() {
			CharT temp;
			do {
				if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT>::tellg() == 0) return Ret::ERR;
				std::basic_fstream<CharT>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT>::peek();
				if (temp == LF) {
					std::basic_fstream<CharT>::get(temp);
					return Ret::ERR;
				}
			} while (temp != comma);

			do {
				if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT>::tellg() == 0) return Ret::BEGIN_OF_CSV;
				std::basic_fstream<CharT>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT>::peek();
				if (temp == LF) {
					std::basic_fstream<CharT>::get(temp);
					return Ret::BEGIN_OF_ROW;
				}
			} while (temp != comma);

			std::basic_fstream<CharT>::get(temp);
			return Ret::OK;
		};

		/**
		前の行の先頭の文字までファイルの入力位置をシークする
		@return 前の行がない場合ERRを返す
		*/
		Ret seekToPrevCol() {
			CharT temp;
			do {
				if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT>::tellg() == 0) return Ret::ERR;
				std::basic_fstream<CharT>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT>::peek();
			} while (temp != LF);

			do {
				if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT>::tellg() == 0) return Ret::BEGIN_OF_CSV;
				std::basic_fstream<CharT>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT>::peek();
			} while (temp != LF);

			std::basic_fstream<CharT>::get(temp);
			return Ret::OK;
		};

		/**
		現在のセルの先頭の文字までファイルの入力位置をシークする
		*/
		Ret seekToCurrCol() {
			CharT temp;
			do {
				if (std::basic_fstream<CharT>::rdstate() != std::basic_fstream<CharT>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT>::tellg() == 0) return Ret::BEGIN_OF_CSV;
				std::basic_fstream<CharT>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT>::peek();
				if (temp == LF) {
					std::basic_fstream<CharT>::get(temp);
					return Ret::BEGIN_OF_ROW;
				}
			} while (temp != comma);

			std::basic_fstream<CharT>::get(temp);
			return Ret::OK;
		};
	};

	typedef basic_CsvStream<char> CsvStream;
	typedef basic_CsvStream<wchar_t> WCsvStream;
}



#endif /* CSVSTREAM_H */

