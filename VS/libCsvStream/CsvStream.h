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

/**
	���]�̌^�œ���̕������擾�ł���֐��̒�`��W�J����
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
	������𕡐��̕����񃊃e�����ɂ��ēW�J����
*/
#define _CSVSTREAM_MULTITYPE_STR(arg) {arg , L##arg , u##arg , U##arg} 

namespace CsvStreamNS {

	/**
		������̐擪�̃X�y�[�X(std::isspace�ŋ󔒂Ƃ݂Ȃ���镶��)���폜����
		@param str �X�y�[�X���폜���镶����ւ̎Q��
		@return str�ւ̎Q��
	*/
	template<class CharT> std::basic_string<CharT>& DeleteFirstSpace(std::basic_string<CharT> &str) {
		if (str.empty()) {											//��̕�����̏ꍇ�A�I��
			return str;
		}
		typename std::basic_string<CharT>::const_iterator strIt;
		std::locale loc = std::locale();

		strIt = str.cbegin();
		if (std::isspace<CharT>(*strIt,loc) == false){				//�ŏ��̕������X�y�[�X���`�F�b�N					
			return str;												//�ŏ��̕������X�y�[�X�łȂ��Ȃ�A�I��
		}
		++strIt;

		for (; (strIt != str.cend()) && (std::isspace<CharT>(*strIt, loc)); ++strIt) {	}	//������̍Ō�ɂȂ邩�A�󔒂łȂ��Ȃ�܂Ń��[�v

		str.erase(str.cbegin(), strIt);							//�ŏ��̕�������󔒂łȂ������̈�܂��܂ł��폜
		return str;
	}
	
	/**
		������̖����̃X�y�[�X(std::isspace�ŋ󔒂Ƃ݂Ȃ���镶��)���폜����
		@param str �X�y�[�X���폜���镶����ւ̎Q��
		@return str�ւ̎Q��
	*/
	template<class CharT> std::basic_string<CharT>& DeleteLastSpace(std::basic_string<CharT> &str) {
		if (str.empty()) {											//��̕�����̏ꍇ�A�I��
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

		for (; (i<size) && (std::isspace<CharT>(str[size-i-1], loc)); i++) {}	//������̍Ō�ɂȂ邩�A�󔒂łȂ��Ȃ�܂Ń��[�v

		str.erase(size - i);										//�󔒂łȂ���������̕������폜
		return str;
	}
	/**
		������̐擪�Ɩ����̃X�y�[�X(std::isspace�ŋ󔒂Ƃ݂Ȃ���镶��)���폜����
		@param str �X�y�[�X���폜���镶����ւ̎Q��
		@return str�ւ̎Q��
	*/
	template<class CharT> std::basic_string<CharT>& DeleteSpace(std::basic_string<CharT> &str) {
		return DeleteLastSpace<CharT>(DeleteFirstSpace<CharT>(str));
	}

	enum class Ret : int {
		OK = 1,				//!<�����ɐ��������B
		END_OF_ROW,			//!<�����ɐ������A���̌��ʁA�s�̍Ō�ɒB����
		BEGIN_OF_ROW,		//!<�����ɐ������A���̌��ʁA�s�̍ŏ��ɒB����
		END_OF_CSV,			//!<�����ɐ������A���̌��ʁA�t�@�C����EOF�ɒB����
		BEGIN_OF_CSV,		//!<�����ɐ������A���̌��ʁA�t�@�C���̐擪�ɒB����
		ERR,				//!<�����Ɏ��s����
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
			���b�Z�[�W�̒�`
		*/
		enum class Msg : int {
			FileOpenError = 0,			//!<�t�@�C���I�[�v���G���[
			MemoryAllocationError,		//!<�������m�ۂȂǂ̃G���[
			BufferSizeError,			//!<�o�b�t�@�̃T�C�Y������Ȃ�
			StringConversionError,		//!<������̕ϊ��Ɏ��s
			Num							//!<���b�Z�[�W�̐����擾���邽�߂̗񋓒萔
		};

		/**
			�����̕�����^���܂Ƃ߂��\���́B_CSVSTREAM_MULTITYPE_STR�Ƒg�ݍ��킹�Ďg�����Ƃ�z��
		*/
		struct MultiTypeMessage {
			const char* c;
			const wchar_t* L;
			const char16_t* u;
			const char32_t* U;
		};

		/**
			�G���[�Ȃǂ̃��b�Z�[�W�z��
		*/
		static constexpr MultiTypeMessage messages[(int)Msg::Num] = {
			_CSVSTREAM_MULTITYPE_STR("Failed to open file."),
			_CSVSTREAM_MULTITYPE_STR("Failed to allocate memory."),
			_CSVSTREAM_MULTITYPE_STR("The buffer is too small."),
			_CSVSTREAM_MULTITYPE_STR("Failed to convert string."),
		};

		/**
			messages���烁�b�Z�[�W���擾����
			@param msg �擾���������b�Z�[�W
		*/
		template<class CharTT> static const CharTT* GetMessage(Msg msg) {
			return messages[(int)msg].c;
		};

		/**
			char�^�œ��ꉻ����GetMessage
			@param msg �擾���������b�Z�[�W
		*/
		template<> static const char* GetMessage<char>(Msg msg) {
			return messages[(int)msg].c;
		};

		/**
			wchar_t�^�œ��ꉻ����GetMessage
			@param msg �擾���������b�Z�[�W
		*/
		template<> static const wchar_t* GetMessage<wchar_t>(Msg msg) {
			return messages[(int)msg].L;
		};
		
		/**
			char16_t�^�œ��ꉻ����GetMessage
			@param msg �擾���������b�Z�[�W
		*/
		template<> static const char16_t* GetMessage<char16_t>(Msg msg) {
			return messages[(int)msg].u;
		};

		/**
			char32_t�^�œ��ꉻ����GetMessage
			@param msg �擾���������b�Z�[�W
		*/
		template<> static const char32_t* GetMessage<char32_t>(Msg msg) {
			return messages[(int)msg].U;
		};

		static constexpr CharT comma = GetComma<CharT>();			//!<�R���}
		static constexpr CharT nullChar = GetNullChar<CharT>();		//!<null����
		static constexpr CharT CR = GetCR<CharT>();					//!<Carriage Return
		static constexpr CharT LF = GetLF<CharT>();					//!<Line Feed

	public:
		basic_CsvStream() {
		};

		/**
			@param filename �J�������t�@�C���̃p�X
			@param mode �t�@�C�����J�����̃��[�h
			@param existCR CR�̗L��
			@param errorOutputStream �G���[�Ȃǂ̃��b�Z�[�W�̏o�͐�B���b�Z�[�W�o�͂��s�v�Ȃ�nullptr�ŗǂ�
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
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::FileOpenError) << ": filename "<< filename << std::endl;
			}

			if (buf == NULL) {
				if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::MemoryAllocationError) << std::endl;
				std::basic_fstream<CharT>::close();
			}
		};

		~basic_CsvStream() {
			delete[] buf;
		};
		
		/**
		�t�@�C���̌��݂̓��͈ʒu�̃Z���̕������ǂݎ��
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
			if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::BufferSizeError) << std::endl;
			return Ret::ERR;
		};
		
		/**
		�t�@�C���̌��݂̓��͈ʒu�̃Z���̕������ǂݎ��
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
			�t�@�C���̌��݂̓��͈ʒu���畡���̃Z���̕������ǂݎ��
			@param des �ǂݎ����������̑�����vector�B
			@param num �ǂݎ�镶����̍ő吔�B0�̏ꍇ�s�̏I���܂œǂݎ��B
		*/
		Ret readCells(std::vector<std::basic_string<CharT>>& des , const typename std::vector<std::basic_string<CharT>>::size_type num = 0) {
			Ret ret;
			typename std::vector<std::basic_string<CharT>>::size_type i;

			if (num > 0) {			//�ǂݎ�镶����̐����w�肳��Ă���ꍇ�A
				des.resize(num);	//���炩���߃��T�C�Y���Ă���
				i = 0;
				do {
					ret = readCell(des[i++]);							//���݂̓��͈ʒu��
					if (ret == CsvStreamNS::Ret::ERR) {					//�G���[�̏ꍇ�I������
						return ret;
					}
				} while ((i<num)&&(ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//�s�̏I��肩�t�@�C���̏I���܂œǂݍ���
				des.resize(i);		//���T�C�Y���ė]���ȗv�f���폜����
			}
			else {
				i = 0;
				do {
					if (i >= des.size()) des.push_back(std::basic_string<CharT>());
					ret = readCell(des[i++]);							//���݂̓��͈ʒu��
					if (ret == CsvStreamNS::Ret::ERR) {					//�G���[�̏ꍇ�I������
						return ret;
					}
				} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (ret != CsvStreamNS::Ret::END_OF_CSV));	//�s�̏I��肩�t�@�C���̏I���܂œǂݍ���
				des.resize(i);		//���T�C�Y���ė]���ȗv�f���폜����
			}
			return ret;
		}

		/**
		�t�@�C���̌��݂̓��͈ʒu�̃Z���̐�����ǂݎ��
		*/
		Ret readCell(int* des,int base = 10) {
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
					DeleteLastSpace(temp);						//�󔒂�����
					*des = std::stoi(temp, &idx, base);			//���l�֕ϊ�
					if (temp.length() != idx) {					//������̍Ō�܂ŕϊ�����Ȃ������ꍇ
						if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::StringConversionError) << "\tstring : " << temp << "\tidx : " << idx << std::endl;
						*des = 0;
						ret = Ret::ERR;
					}
				}
				catch (std::invalid_argument &e) {
					if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::StringConversionError) << "\tstring : " << temp << "\tidx : " << idx << std::endl;
					if (errOutputStream != nullptr) *errOutputStream << e.what() << std::endl;
					*des = 0;
					ret = Ret::ERR;
				}
				catch (std::out_of_range &e) {
					if (errOutputStream != nullptr) *errOutputStream << GetMessage<CharT>(Msg::StringConversionError) << "\tstring : " << temp << "\tidx : " << idx << std::endl;
					if (errOutputStream != nullptr) *errOutputStream << e.what() << std::endl;
					*des = 0;
					ret = Ret::ERR;
				}
				return ret;
				break;
			case Ret::ERR:
			default:
				*des = 0;
				ret = Ret::ERR;
				return ret;
				break;
			}
		};

		/**
		�s���̎��̃Z���̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return ���̃Z�����Ȃ��ꍇERR��Ԃ�
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
		���̍s�̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return ���̍s���Ȃ��ꍇERR��Ԃ�
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
		�s���̑O�̃Z���̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return �O�̃Z�����Ȃ��ꍇERR��Ԃ�
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
		�O�̍s�̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return �O�̍s���Ȃ��ꍇERR��Ԃ�
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
		���݂̃Z���̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
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

