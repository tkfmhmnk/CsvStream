/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   csvstream.h
 * Author: hamanaka
 *
 * Created on 2018/12/09, 0:57
 */

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

class csvstream : public std::fstream{
public:
    csvstream(){}       
    csvstream(const char* filename,std::ios_base::openmode mode);
    ~csvstream();
    CsvResultType getcell(char* des,std::streamsize n);
    CsvResultType getcell(int* des);
private:
    char* buf;
    static const int bufsize=1000;
};


#endif /* CSVSTREAM_H */

