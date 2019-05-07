/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"csvstream.h"
#include<iostream>
using namespace std;

 csvstream::csvstream(const char* filename,ios_base::openmode mode = ios_base::in | ios_base::out)
         : fstream(filename,mode)
 {
     buf = new char[bufsize];
     if(buf == NULL){
         cout << "memory allocation failure" << endl;
         exit(1);
     }
 }
 
 csvstream::~csvstream(){
     delete[] buf;
 }
 //現在の位置のセルを文字列で読み込む
CsvResultType csvstream::getcell(char* des,streamsize n){
    char temp;
    while((n--)>1){
        get(temp);
        if(rdstate() != goodbit){
            *des = '\0';
            clear();
            return END_OF_CSV;
        }else if(temp == ','){
            *des = '\0';
            return NO_ERROR;
        }else if(temp == '\n'){
            #ifdef CR_IS_EXIST
                *(des-1) = '\0';
            #else
                *des = '\0';
            #endif
            return END_OF_ROW;
        }
        *(des++) = temp;
    }
    *des = '\0';
    cout <<"The buffer is too small." << endl;
    return ERROR;
}
 
 //現在の位置のセルを整数で読み込む
CsvResultType csvstream::getcell(int* des){
    CsvResultType ret;
    ret = getcell(buf,bufsize);
    switch(ret){
        case NO_ERROR:
        case END_OF_ROW:
        case END_OF_CSV:
            *des = atoi(buf);
            return ret;
            break;
        case ERROR:
        default:
            *des = 0;
            return ret;
            break;
    }
}