#include<iostream>
#include"../libCsvStream/CsvStream.h"

using namespace std;

#define EXECUTE_TEST(arg) std::cout << #arg << arg << std::endl

const char* testdatas[] = {
	"..\\..\\TestData\\test1.csv"
};

bool test1() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;
	
	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW)&&(csv1.eof()==false));

	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.close();

	return false;

}

bool test2() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;

	csv1.seekToNextCell();

	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	if (csv1.seekToNextCol() == CsvStreamNS::Ret::END_OF_CSV) {
		csv1.close();
		return true;
	}
	else {
		csv1.close();
		return false;
	}
}

bool test3() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;

	csv1.seekToNextCol();

	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.close();
	return false;
}


bool test4() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;

	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.seekToPrevCol();

	i = 0;
	do {
		ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.close();
	return false;
}

bool test5() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;

	i = 0;
	ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));


	csv1.seekToPrevCell();
	i = 1;
	ret = csv1.readCell(data[i], sizeof(data[i]) / sizeof(char));

	csv1.close();
	return false;
}

bool test6() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	string data[20];
	ret = CsvStreamNS::Ret::OK;

	i = 0;
	do {
		ret = csv1.readCell(data[i]);
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));


	do {
		ret = csv1.readCell(data[i]);
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));


	do {
		ret = csv1.readCell(data[i]);
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	if(data[0] == "1234567")
	if(data[1] == "2")
	if(data[2] == "3123")
	if(data[3] == "4")
	if (data[4] == "5")
	if (data[5] == "6")
	if (data[6].empty() == true)
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

bool test7() {
	CsvStreamNS::CsvStream csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}



	int i;
	string data[20];
	ret = CsvStreamNS::Ret::OK;

	i = 0;
	do {
		ret = csv1.readCell(data[i]);
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));
	int tell = csv1.tellp();
	csv1.seekp(csv1.tellg());	//シークしないとファイル先頭に書き込まれてしまう。tellpの値は当てにならない
	tell = csv1.tellp();
	csv1.put('A');
	csv1.put('A');
	csv1.put('A');
	csv1.put('A');
	tell = csv1.tellp();
	int status = csv1.rdstate();
	csv1.flush();				//flushしないと反映されない。
	csv1.seekToPrevCol();

	do {
		ret = csv1.readCell(data[i]);
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));


	do {
		ret = csv1.readCell(data[i]);
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	if(data[0] == "1234567")
	if(data[1] == "2")
	if(data[2] == "3123")
	if(data[3] == "4")
	if (data[4] == "5")
	if (data[5] == "6")
	if (data[6].empty() == true){
		csv1.close();
		return true;
	}

	csv1.close();
	return false;

}



int main() {
	fstream test("A.csv");
	if (test.is_open()) {
		test.put('D');
	}
	test.close();
	EXECUTE_TEST(test1());
	EXECUTE_TEST(test2());
	EXECUTE_TEST(test3());
	EXECUTE_TEST(test4());
	EXECUTE_TEST(test5());
	EXECUTE_TEST(test6());
	EXECUTE_TEST(test7());
	return 0;
}