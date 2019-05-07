#include <cstdlib>
#include<iostream>
#include<sstream>
#include<iomanip>
#include"csvstream.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    csvstream csv("test.csv",ios_base::in);
    char buf[3];
    csv.getcell(buf,3);
    cout << buf << endl;
    csv.getcell(buf,3);
    cout << buf << endl;
    csv.getcell(buf,15);
    cout << buf << endl;
    csv.getcell(buf,15);
    cout << buf << endl;
    csv.getcell(buf,15);
    cout << buf << endl;
    csv.getcell(buf,15);
    cout << buf << endl;
        csv.getcell(buf,15);
    cout << buf << endl;
    csv.getcell(buf,15);
    cout << buf << endl;
    csv.getcell(buf,15);
    cout << buf << endl;
    csv.close();
    return 0;
}

