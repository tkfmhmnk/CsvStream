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
#include<stdexcept>
#include<locale>
#include<vector>
#include<iomanip>

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

	/**
		文字列の先頭のスペース(std::isspaceで空白とみなされる文字)を削除する
		@param str スペースを削除する文字列への参照
		@return strへの参照
	*/
	template<class CharT> std::basic_string<CharT>& DeleteFirstSpace(std::basic_string<CharT> &str) {
		if (str.empty()) {											//空の文字列の場合、終了
			return str;
		}
		typename std::basic_string<CharT>::const_iterator strIt;
		std::locale loc = std::locale();

		strIt = str.cbegin();
		if (std::isspace<CharT>(*strIt,loc) == false){				//最初の文字がスペースがチェック					
			return str;												//最初の文字がスペースでないなら、終了
		}
		++strIt;

		for (; (strIt != str.cend()) && (std::isspace<CharT>(*strIt, loc)); ++strIt) {	}	//文字列の最後になるか、空白でなくなるまでループ

		str.erase(str.cbegin(), strIt);							//最初の文字から空白でない文字の一つまえまでを削除
		return str;
	}
	
	/**
		文字列の末尾のスペース(std::isspaceで空白とみなされる文字)を削除する
		@param str スペースを削除する文字列への参照
		@return strへの参照
	*/
	template<class CharT> std::basic_string<CharT>& DeleteLastSpace(std::basic_string<CharT> &str) {
		if (str.empty()) {											//空の文字列の場合、終了
			return str;
		}

		std::locale loc = std::locale();
		typename std::basic_string<CharT>::size_type i;
		typename std::basic_string<CharT>::size_type size = str.length();
		i = 0;
		if (std::isspace<CharT>(str[size - i - 1], loc) == false) {
			return str;
		}
		i++;

		for (; (i<size) && (std::isspace<CharT>(str[size-i-1], loc)); i++) {}	//文字列の最後になるか、空白でなくなるまでループ

		str.erase(size - i);										//空白でない文字より後の文字を削除
		return str;
	}
	/**
		文字列の先頭と末尾のスペース(std::isspaceで空白とみなされる文字)を削除する
		@param str スペースを削除する文字列への参照
		@return strへの参照
	*/
	template<class CharT> std::basic_string<CharT>& DeleteSpace(std::basic_string<CharT> &str) {
		return DeleteLastSpace<CharT>(DeleteFirstSpace<CharT>(str));
	}

	/**
		テンプレート化したsto系の文字列-数値変換関数
		@param str string型の文字列
		@param idx 変換に使用されなかった要素のインデックスの代入先
		@param base 整数の数値変換における数値の基数
		@return 変換結果の数値
		@todo charやwchar_t以外の文字を扱えるようにしなければらない
	*/
	template<class NumericT> NumericT stoNumeric(const std::basic_string<char> &str, std::size_t* idx = nullptr, int base = 10) {
		return (NumericT)(std::stoi(str, idx, base));
	}

	/**
		テンプレート化したsto系の文字列-数値変換関数
		@param str wstring型の文字列
		@param idx 変換に使用されなかった要素のインデックスの代入先
		@param base 整数の数値変換における数値の基数
		@return 変換結果の数値
	*/
	template<class NumericT> NumericT stoNumeric(const std::basic_string<wchar_t> &str, std::size_t* idx = nullptr, int base = 10) {
		return (NumericT)(std::stoi(str, idx, base));
	}

	/**
		int型で特殊化したstoNumeric
	*/
	template<> int stoNumeric<int>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		long型で特殊化したstoNumeric
	*/
	template<> long stoNumeric<long>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		long long型で特殊化したstoNumeric
	*/
	template<> long long stoNumeric<long long>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		unsigned long型で特殊化したstoNumeric
	*/
	template<> unsigned long stoNumeric<unsigned long>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		unsigned long long型で特殊化したstoNumeric
	*/
	template<> unsigned long long stoNumeric<unsigned long long>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		float型で特殊化したstoNumeric
	*/
	template<> float stoNumeric<float>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		double型で特殊化したstoNumeric
	*/
	template<> double stoNumeric<double>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		long double型で特殊化したstoNumeric
	*/
	template<> long double stoNumeric<long double>(const std::basic_string<char> &str, std::size_t* idx, int base);

	/**
		int型で特殊化したstoNumeric
	*/
	template<> int stoNumeric<int>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		long型で特殊化したstoNumeric
	*/
	template<> long stoNumeric<long>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		long long型で特殊化したstoNumeric
	*/
	template<> long long stoNumeric<long long>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		unsigned long型で特殊化したstoNumeric
	*/
	template<> unsigned long stoNumeric<unsigned long>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		unsinged long long型で特殊化したstoNumeric
	*/
	template<> unsigned long long stoNumeric<unsigned long long>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		float型で特殊化したstoNumeric
	*/
	template<> float stoNumeric<float>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		double型で特殊化したstoNumeric
	*/
	template<> double stoNumeric<double>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

	/**
		long double型で特殊化したstoNumeric
	*/
	template<> long double stoNumeric<long double>(const std::basic_string<wchar_t> &str, std::size_t* idx, int base);

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



	template<class CharT, class Traits = std::char_traits<CharT>> class basic_CsvStream : public std::basic_fstream<CharT, Traits>{
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
			StringConversionError,		//!<文字列の変換に失敗
			InvalidArgument,			//!<不正な引数
			Num							//!<メッセージの数を取得するための列挙定数
		};

		/**
			複数の文字列型をまとめた構造体。_CSVSTREAM_MULTITYPE_STRと組み合わせて使うことを想定
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
		static constexpr MultiTypeMessage messages[(int)Msg::Num] = {
			_CSVSTREAM_MULTITYPE_STR("Failed to open file."),
			_CSVSTREAM_MULTITYPE_STR("Failed to allocate memory."),
			_CSVSTREAM_MULTITYPE_STR("The buffer is too small."),
			_CSVSTREAM_MULTITYPE_STR("Failed to convert string."),
			_CSVSTREAM_MULTITYPE_STR("Invalid argument.")
		};

		/**
			messagesからメッセージを取得する
			@param msg 取得したいメッセージ
		*/
		template<class CharTT> static const CharTT* GetMessage(Msg msg) {
			return messages[(int)msg].c;
		};

		/**
			char型で特殊化したGetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const char* GetMessage<char>(Msg msg) {
			return messages[(int)msg].c;
		};

		/**
			wchar_t型で特殊化したGetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const wchar_t* GetMessage<wchar_t>(Msg msg) {
			return messages[(int)msg].L;
		};
		
		/**
			char16_t型で特殊化したGetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const char16_t* GetMessage<char16_t>(Msg msg) {
			return messages[(int)msg].u;
		};

		/**
			char32_t型で特殊化したGetMessage
			@param msg 取得したいメッセージ
		*/
		template<> static const char32_t* GetMessage<char32_t>(Msg msg) {
			return messages[(int)msg].U;
		};

		static constexpr CharT comma = GetComma<CharT>();			//!<コンマ
		static constexpr CharT nullChar = GetNullChar<CharT>();		//!<null文字
		static constexpr CharT CR = GetCR<CharT>();					//!<Carriage Return
		static constexpr CharT LF = GetLF<CharT>();					//!<Line Feed

	public:
		basic_CsvStream() {
		};

		/**
			@param filename 開きたいファイルのパス
			@param mode ファイルを開く時のモード
			@param existCR CRの有無
			@param errorOutputStream エラーなどのメッセージの出力先。メッセージ出力が不要ならnullptrで良い
		*/
		basic_CsvStream(
			const CharT* filename,
			std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out ,
			bool existCR = true,
			std::basic_ostream<CharT>* errorOutputStream = nullptr) : std::basic_fstream<CharT,Traits>(filename, mode)
		{
			buf = new CharT[bufsize];
			errOutputStream = errorOutputStream;
			_CR = existCR;

			if (std::basic_fstream<CharT,Traits>::is_open() == false) {
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::FileOpenError) << ": filename "<< filename << std::endl;
			}

			if (buf == NULL) {
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::MemoryAllocationError) << std::endl;
				std::basic_fstream<CharT,Traits>::close();
			}
		};

		~basic_CsvStream() {
			delete[] buf;
		};
		
		/**
		ファイルの現在の入力位置のセルの文字列を読み取る
		*/
		Ret readCell_c_str(CharT* des, std::streamsize n) {
			CharT temp;
			if (seekToCurrCol() == Ret::ERR) return Ret::ERR;
			while ((n--) > 1) {
				std::basic_fstream<CharT,Traits>::get(temp);
				if (std::basic_fstream<CharT,Traits>::eof()) {
					*des = nullChar;
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) {
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
			if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::BufferSizeError) << std::endl;
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
				std::basic_fstream<CharT,Traits>::get(temp);
				if (std::basic_fstream<CharT,Traits>::eof()) {
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) {
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
			ファイルの現在の入力位置から複数のセルの文字列を読み取る
			@param des 読み取った文字列の代入先のvector。
			@param num 読み取る文字列の最大数。0の場合行の終わりかファイルの終わりに達するまで読み取る。
			@return 正常はEND_OF_ROWかEND_OF_CSVが返るが、読み取った文字列の数が指定したnumに達した場合はOKが返る。異常時はERRが返る
		*/
		Ret readCells(std::vector<std::basic_string<CharT>>& des , const typename std::vector<std::basic_string<CharT>>::size_type num = 0) {
			Ret ret;
			typename std::vector<std::basic_string<CharT>>::size_type i;

			if (num > 0) {			//読み取る文字列の数が指定されている場合、
				des.resize(num);	//あらかじめリサイズしておく
				i = 0;
				do {
					ret = readCell(des[i++]);							//現在の入力位置の文字列を読み込む
					if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
						return ret;
					}
				} while ((i<num)&&(ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりか、指定の数になるまで読み込む
				des.resize(i);		//リサイズして余分な要素を削除する
			}
			else {
				i = 0;
				do {
					if (i >= des.size()) des.push_back(std::basic_string<CharT>());
					ret = readCell(des[i++]);							//現在の入力位置の文字列を読み込む
					if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
						return ret;
					}
				} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりまで読み込む
				des.resize(i);		//リサイズして余分な要素を削除する
			}
			return ret;
		}

		/**
			ファイルの現在の入力位置から複数のセルの文字列を読み取る
			@param des 読み取った文字列の代入先の配列のポインタ。
			@param num 読み取る文字列の最大数。
			@return 正常はEND_OF_ROWかEND_OF_CSVかOKが返る。異常時はERRが返る
		*/
		Ret readCells(std::basic_string<CharT>* des, const int num) {
			Ret ret;
			int i;

			if ((num <= 0)||(des == nullptr)) {
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::InvalidArgument) << "\tnum : " << num << "\tdes ; " << (int)des << std::endl;
				return Ret::ERR;
			}

			i = 0;
			do {
				ret = readCell(des[i++]);							//現在の入力位置の文字列を読み込む
				if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
					return ret;
				}
			} while ((i < num) && (ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりか、指定の数になるまで読み込む
			for (; i < num; i++) des[i].clear();					//余分な要素はクリアしておく

			return ret;
		}

		/**
			ファイルの現在の入力位置から複数のセルの文字列を読み取る
			@param des 読み取った文字列の代入先の配列のポインタ。
			@param num 読み取る文字列の最大数。
			@param n 読み取る文字列の文字数の最大数
			@return 正常はEND_OF_ROWかEND_OF_CSVかOKが返る。異常時はERRが返る
		*/
		Ret readCells_c_str(CharT* des, const int num,const std::streamsize n) {
			Ret ret;
			int i;

			if ((num <= 0)||(n<=0)||(des == nullptr)) {
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::InvalidArgument) << "\tnum : " << num <<"\tn : " << n << "\tdes ; " << (int)des << std::endl;
				return Ret::ERR;
			}

			i = 0;
			do {
				ret = readCell_c_str(des+(i++)*n,n);						//現在の入力位置の文字列を読み込む
				if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
					return ret;
				}
			} while ((i < num) && (ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりか、指定の数になるまで読み込む
			for (; i < num; i++) *(des+(i)*n) = nullChar;			//余分な要素は初期化しておく

			return ret;
		}

		/**
		ファイルの現在の入力位置のセルの数値をNumericT型で読み取る
		*/
		template<class NumericT> Ret readCell(NumericT& des,int base = 10) {
			if (seekToCurrCol() == Ret::ERR) return Ret::ERR;
			Ret ret;
			std::basic_string<CharT> temp;
			std::size_t idx;
			ret = readCell(temp);
			switch (ret) {
			case Ret::OK:
			case Ret::END_OF_ROW:
			case Ret::END_OF_CSV:
				try {
					DeleteLastSpace(temp);								//空白を除去
					des = stoNumeric<NumericT>(temp, &idx, base);		//数値へ変換
					if (temp.length() != idx) {							//文字列の最後まで変換されなかった場合
						if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::StringConversionError) << "\tstring : " << temp << "\tidx : " << idx << std::endl;
						des = 0;
						ret = Ret::ERR;
					}
				}
				catch (std::invalid_argument &e) {
					if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::StringConversionError) << "\tstring : " << temp << "\tidx : " << idx << std::endl;
					if (errOutputStream != nullptr) *errOutputStream << e.what() << std::endl;
					des = 0;
					ret = Ret::ERR;
				}
				catch (std::out_of_range &e) {
					if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::StringConversionError) << "\tstring : " << temp << "\tidx : " << idx << std::endl;
					if (errOutputStream != nullptr) *errOutputStream << e.what() << std::endl;
					des = 0;
					ret = Ret::ERR;
				}
				return ret;
				break;
			case Ret::ERR:
			default:
				des = 0;
				ret = Ret::ERR;
				return ret;
				break;
			}
		};

		/**
			ファイルの現在の入力位置から複数のセルの数値をNumericT型で読み取る
			@param des 読み取った数値の代入先のvector。
			@param num 読み取る数値の最大数。0の場合行の終わりかファイルの終わりに達するまで読み取る。
			@param base 整数の数値変換における数値の基数
			@return 正常はEND_OF_ROWかEND_OF_CSVが返るが、読み取った数値の数が指定したnumに達した場合はOKが返る。異常時はERRが返る
		*/
		template<class NumericT> Ret readCells(std::vector<NumericT>& des , const typename std::vector<NumericT>::size_type num = 0,const int base = 10) {
			Ret ret;
			typename std::vector<NumericT>::size_type i;

			if (num > 0) {			//読み取る文字列の数が指定されている場合、
				des.resize(num);	//あらかじめリサイズしておく
				i = 0;
				do {
					ret = readCell(des[i++],base);						//現在の入力位置の文字列を読み込む
					if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
						return ret;
					}
				} while ((i<num)&&(ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりか、指定の数になるまで読み込む
				des.resize(i);		//リサイズして余分な要素を削除する
			}
			else {
				i = 0;
				do {
					if (i >= des.size()) des.push_back(NumericT());
					ret = readCell(des[i++],base);						//現在の入力位置の文字列を読み込む
					if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
						return ret;
					}
				} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりまで読み込む
				des.resize(i);		//リサイズして余分な要素を削除する
			}
			return ret;
		}

		/**
			ファイルの現在の入力位置から複数のセルの数値を読み取る
			@param des 読み取った数値の代入先の配列のポインタ。
			@param num 読み取る数値の最大数。
			@param base 整数の数値変換における数値の基数
			@return 正常はEND_OF_ROWかEND_OF_CSVかOKが返る。異常時はERRが返る
		*/
		template<class NumericT> Ret readCells(NumericT* des, const int num,const int base = 10) {
			Ret ret;
			int i;

			if ((num <= 0) || (des == nullptr)) {
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::InvalidArgument) << "\tnum : " << num << "\tdes ; " << (int)des << std::endl;
				return Ret::ERR;
			}

			i = 0;
			do {
				ret = readCell(des[i++],base);						//現在の入力位置の文字列を読み込む
				if (ret == CsvStreamNS::Ret::ERR) {					//エラーの場合終了する
					return ret;
				}
			} while ((i < num) && (ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//行の終わりかファイルの終わりか、指定の数になるまで読み込む
			for (; i < num; i++) des[i]=0;							//余分な要素はクリアしておく

			return ret;
		}

		/**
		行中の次のセルの先頭の文字までファイルの入力位置をシークする
		@return 次のセルがない場合ERRを返す
		*/
		Ret seekToNextCell() {
			CharT temp;
			while (1) {
				std::basic_fstream<CharT,Traits>::get(temp);
				if (std::basic_fstream<CharT,Traits>::eof()) {
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) {
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
				std::basic_fstream<CharT,Traits>::ignore(numeric_limits<std::streamsize>::max(), LF);
				if (std::basic_fstream<CharT,Traits>::eof()) {
					return Ret::END_OF_CSV;
				}
				else if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) {
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
				if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT,Traits>::tellg() == 0) return Ret::ERR;
				std::basic_fstream<CharT,Traits>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT,Traits>::peek();
				if (temp == LF) {
					std::basic_fstream<CharT,Traits>::get(temp);
					return Ret::ERR;
				}
			} while (temp != comma);

			do {
				if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT,Traits>::tellg() == 0) return Ret::BEGIN_OF_CSV;
				std::basic_fstream<CharT,Traits>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT,Traits>::peek();
				if (temp == LF) {
					std::basic_fstream<CharT,Traits>::get(temp);
					return Ret::BEGIN_OF_ROW;
				}
			} while (temp != comma);

			std::basic_fstream<CharT,Traits>::get(temp);
			return Ret::OK;
		};

		/**
		前の行の先頭の文字までファイルの入力位置をシークする
		@return 前の行がない場合ERRを返す
		*/
		Ret seekToPrevCol() {
			CharT temp;
			do {
				if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT,Traits>::tellg() == 0) return Ret::ERR;
				std::basic_fstream<CharT,Traits>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT,Traits>::peek();
			} while (temp != LF);

			do {
				if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT,Traits>::tellg() == 0) return Ret::BEGIN_OF_CSV;
				std::basic_fstream<CharT,Traits>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT,Traits>::peek();
			} while (temp != LF);

			std::basic_fstream<CharT,Traits>::get(temp);
			return Ret::OK;
		};

		/**
		現在のセルの先頭の文字までファイルの入力位置をシークする
		*/
		Ret seekToCurrCol() {
			CharT temp;
			do {
				if (std::basic_fstream<CharT,Traits>::rdstate() != std::basic_fstream<CharT,Traits>::goodbit) return Ret::ERR;

				if (std::basic_fstream<CharT,Traits>::tellg() == 0) return Ret::BEGIN_OF_CSV;
				std::basic_fstream<CharT,Traits>::seekg(-1, ios_base::cur);
				temp = std::basic_fstream<CharT,Traits>::peek();
				if (temp == LF) {
					std::basic_fstream<CharT,Traits>::get(temp);
					return Ret::BEGIN_OF_ROW;
				}
			} while (temp != comma);

			std::basic_fstream<CharT,Traits>::get(temp);
			return Ret::OK;
		};

		/**
			書き込み位置を読み込み位置にする
			@todo 書き込み位置と読み込み位置の関係について調査する。
			tellpで取得した通りの位置に書き込みできず、seekpでシークするとtellpの値が同じにも関わらず、tellpで取得した通りの位置に書き込みできるようになる。
		*/
		void seekPutPosToGetPos() {
			std::basic_fstream<CharT,Traits>::seekp(std::basic_fstream<CharT,Traits>::tellg());
		}

		template<class T> basic_CsvStream<CharT, Traits>& writeCells(const std::vector<T>& src) {
			typename std::vector<T>::const_iterator srcIt;
			seekPutPosToGetPos();				//書き込み位置を修正
			
			srcIt = src.cbegin();
			std::basic_ostream<CharT>::operator<<(*srcIt);
			++srcIt;
			for (; srcIt != src.cend(); ++srcIt) {
				std::operator<<(*this, comma);
				std::basic_fstream<CharT,Traits>::operator<<(*srcIt);
			}
			return *this;
		}

		// マニピュレータの実行
		basic_CsvStream<CharT, Traits>& operator<<(std::basic_ostream<CharT,Traits>& (*pf)(std::basic_ostream<CharT,Traits>&)) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(pf);
		};
		basic_CsvStream<CharT, Traits>& operator<<(std::basic_ios<CharT, Traits>& (*pf)(std::basic_ios<CharT, Traits>&)) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(pf);
		};
		basic_CsvStream<CharT, Traits>& operator<<(std::ios_base& (*pf)(std::ios_base&)) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(pf);
		};

		// bool値・数値・ポインタの書式化出力
		basic_CsvStream<CharT, Traits>& operator<<(bool n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(short n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(unsigned short n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(int n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(unsigned int n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(long n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(unsigned long n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(long long n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(unsigned long long n) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(n);
		};
		basic_CsvStream<CharT, Traits>& operator<<(float f) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(f);
		};
		basic_CsvStream<CharT, Traits>& operator<<(double f) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(f);
		};
		basic_CsvStream<CharT, Traits>& operator<<(long double f) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(f);
		};
		basic_CsvStream<CharT, Traits>& operator<<(const void* p) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(p);
		};
		basic_CsvStream<CharT, Traits>& operator<<(nullptr_t) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(nullptr_t);
		};

		// ストリームバッファの非書式化出力
		basic_CsvStream<CharT, Traits>& operator<<(std::basic_streambuf<CharT>* sb) {
			return (basic_CsvStream<CharT, Traits>&)std::basic_fstream<CharT,Traits>::operator<<(sb);
		}

		template<class T> basic_CsvStream<CharT, Traits>& operator<<(const std::vector<T>& src) {
			return writeCells(src);
		}
	};

	typedef basic_CsvStream<char> CsvStream;
	typedef basic_CsvStream<wchar_t> WCsvStream;

}


namespace std {
	// 文字の書式化出力
	template<class CharT, class Traits>
	CsvStreamNS::basic_CsvStream<CharT, Traits>& operator<<(CsvStreamNS::basic_CsvStream<CharT, Traits>& os, CharT c) {
		return (CsvStreamNS::basic_CsvStream<CharT, Traits>&)operator<<((std::basic_fstream<CharT,Traits>&)os, c);
	};
	template<class CharT, class Traits>
	CsvStreamNS::basic_CsvStream<CharT, Traits>& operator<<(CsvStreamNS::basic_CsvStream<CharT, Traits>& os, char c) {
		return (CsvStreamNS::basic_CsvStream<CharT, Traits>&)operator<<((std::basic_fstream<CharT,Traits>&)os, c);
	};
	template<class Traits>
	CsvStreamNS::basic_CsvStream<char, Traits>& operator<<(CsvStreamNS::basic_CsvStream<char, Traits>& os, char c) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, c);
	};
	template<class Traits>
	CsvStreamNS::basic_CsvStream<char, Traits>& operator<<(CsvStreamNS::basic_CsvStream<char, Traits>& os, unsigned char c) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, c);
	};
	template<class Traits>
	CsvStreamNS::basic_CsvStream<char, Traits>& operator<<(CsvStreamNS::basic_CsvStream<char, Traits>& os, signed char c) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, c);
	};
	// 文字列の書式化出力
	template<class CharT, class Traits>
	CsvStreamNS::basic_CsvStream<CharT, Traits>& operator<<(CsvStreamNS::basic_CsvStream<CharT, Traits>& os, const CharT* s) {
		return (CsvStreamNS::basic_CsvStream<CharT, Traits>&)operator<<((std::basic_fstream<CharT,Traits>&)os, s);
	};
	template<class CharT, class Traits>
	CsvStreamNS::basic_CsvStream<CharT, Traits>& operator<<(CsvStreamNS::basic_CsvStream<CharT, Traits>& os, const char* s) {
		return (CsvStreamNS::basic_CsvStream<CharT, Traits>&)operator<<((std::basic_fstream<CharT,Traits>&)os, s);
	};
	template<class Traits>
	CsvStreamNS::basic_CsvStream<char, Traits>& operator<<(CsvStreamNS::basic_CsvStream<char, Traits>& os, const char* s) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, s);
	};
	template<class Traits>
	CsvStreamNS::basic_CsvStream<char, Traits>& operator<<(CsvStreamNS::basic_CsvStream<char, Traits>& os, const unsigned char* s) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, s);
	};
	template<class Traits>
	CsvStreamNS::basic_CsvStream<char, Traits>& operator<<(CsvStreamNS::basic_CsvStream<char, Traits>& os, const signed char* s) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, s);
	};

	// 右辺値参照ストリームへの出力 (C++11)
	template<class CharT, class Traits, class T>
	CsvStreamNS::basic_CsvStream<CharT, Traits>& operator<<(CsvStreamNS::basic_CsvStream<CharT, Traits>&& os, const T& x) {
		return (CsvStreamNS::basic_CsvStream<char, Traits>&)operator<<((std::basic_fstream<char,Traits>&)os, x);
	};
}


#endif /* CSVSTREAM_H */

