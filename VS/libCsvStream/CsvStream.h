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
		OK = 1,				//!<�����ɐ��������B
		END_OF_ROW,			//!<�����ɐ������A���̌��ʁA�s�̍Ō�ɒB����
		BEGIN_OF_ROW,		//!<�����ɐ������A���̌��ʁA�s�̍ŏ��ɒB����
		END_OF_CSV,			//!<�����ɐ������A���̌��ʁA�t�@�C����EOF�ɒB����
		BEGIN_OF_CSV,		//!<�����ɐ������A���̌��ʁA�t�@�C���̐擪�ɒB����
		ERR,				//!<�����Ɏ��s����
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
		�t�@�C���̌��݂̓��͈ʒu�̃Z���̕������ǂݎ��
		*/
		Ret readCell(char* des, std::streamsize n);
		
		/**
		�t�@�C���̌��݂̓��͈ʒu�̃Z���̕������ǂݎ��
		*/
		Ret readCell(std::string& des);

		/**
		�t�@�C���̌��݂̓��͈ʒu�̃Z���̐�����ǂݎ��
		*/
		Ret readCell(int* des);

		/**
		�s���̎��̃Z���̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return ���̃Z�����Ȃ��ꍇERR��Ԃ�
		*/
		Ret seekToNextCell();

		/**
		���̍s�̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return ���̍s���Ȃ��ꍇERR��Ԃ�
		*/
		Ret seekToNextCol();

		/**
		�s���̑O�̃Z���̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return �O�̃Z�����Ȃ��ꍇERR��Ԃ�
		*/
		Ret seekToPrevCell();

		/**
		�O�̍s�̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
		@return �O�̍s���Ȃ��ꍇERR��Ԃ�
		*/
		Ret seekToPrevCol();

		/**
		���݂̃Z���̐擪�̕����܂Ńt�@�C���̓��͈ʒu���V�[�N����
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

