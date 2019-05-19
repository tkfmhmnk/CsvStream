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
//#define CR_IS_EXIST
enum CsvResultType{
    NO_ERROR = 1,
    END_OF_ROW,
    END_OF_CSV,
    ERROR
};

class CsvStream : public std::fstream{
public:
    CsvStream(){}       
    CsvStream(const char* filename,std::ios_base::openmode mode);
    ~CsvStream();
    CsvResultType getcell(char* des,std::streamsize n);
    CsvResultType getcell(int* des);
private:
    char* buf;
    static const int bufsize=1000;
};


#endif /* CSVSTREAM_H */

