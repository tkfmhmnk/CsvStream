#include<iostream>
#include"../libCsvStream/CsvStream.h"

using namespace std;

#define EXECUTE_TEST(arg) std::cout << #arg << "\tresult : "<< arg << std::endl

const char* testdatas[] = {
	"..\\..\\TestData\\test1.csv",
	"..\\..\\TestData\\test2.csv"
};

bool test1() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;
	
	i = 0;
	do {
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW)&&(csv1.eof()==false));

	i = 0;
	do {
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	i = 0;
	do {
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.close();

	return false;

}

bool test2() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
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
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
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
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
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
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.close();
	return false;
}


bool test4() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;

	i = 0;
	do {
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.seekToPrevCol();

	i = 0;
	do {
		ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));
		i++;
	} while ((ret != CsvStreamNS::Ret::END_OF_ROW) && (csv1.eof() == false));

	csv1.close();
	return false;
}

bool test5() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	char data[4][100];
	ret = CsvStreamNS::Ret::OK;

	i = 0;
	ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));


	csv1.seekToPrevCell();
	i = 1;
	ret = csv1.readCell_c_str(data[i], sizeof(data[i]) / sizeof(char));

	csv1.close();
	return false;
}

bool test6() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
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
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
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

bool test8() {
	CsvStreamNS::basic_CsvStream<wchar_t> csv1(L"..\\..\\TestData\\test61.csv", std::ios_base::in | std::ios_base::out, true, &std::wcout);
	return false;
}

bool test9() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0], std::ios_base::in | std::ios_base::out, true, &std::cout);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	int data[20];
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

	if(data[0] == 1234567)
	if(data[1] == 2)
	if(data[2] == 3123)
	if(data[3] == 4)
	if (data[4] == 5)
	if (data[5] == 6)
	if (data[6] == 0)
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

bool testDeleteFirstSpace1() {
	std::string src("   あああ");
	std::string ans("あああ");
	CsvStreamNS::DeleteFirstSpace(src);
	if (src == ans)
		return true;

	return false;
}

bool testDeleteFirstSpace2() {
	std::string src("   a");
	std::string ans("a");
	CsvStreamNS::DeleteFirstSpace(src);
	if (src == ans)
		return true;

	return false;
}


bool testDeleteLastSpace1() {
	std::string src("あああ  ");
	std::string ans("あああ");
	CsvStreamNS::DeleteLastSpace(src);
	if (src == ans)
		return true;

	return false;
}

bool testDeleteLastSpace2() {
	std::string src("a ");
	std::string ans("a");
	CsvStreamNS::DeleteLastSpace(src);
	if (src == ans)
		return true;

	return false;
}

bool testDeleteSpace1() {
	std::string src(" \t あああ \t  ");
	std::string ans("あああ");
	CsvStreamNS::DeleteSpace(src);
	if (src == ans)
		return true;

	return false;
}

bool testDeleteSpace2() {
	std::string src(" \t a \t ");
	std::string ans("a");
	CsvStreamNS::DeleteSpace(src);
	if (src == ans)
		return true;

	return false;
}

bool test10() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[1], std::ios_base::in | std::ios_base::out, true, &std::cout);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	int data[20];
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

	if(data[0] == 1234567)
	if(data[1] == 2)
	if(data[2] == 3123)
	if(data[3] == 4)
	if (data[4] == 5)
	if (data[5] == 6)
	if (data[6] == 0)
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

bool test11() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
	CsvStreamNS::Ret ret1,ret2,ret3;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	std::vector<string> data1;
	std::vector<string> data2;
	std::vector<string> data3;


	ret1 = csv1.readCells(data1);
	ret2 = csv1.readCells(data2);
	ret3 = csv1.readCells(data3);

	if(ret1 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret2 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret3 == CsvStreamNS::Ret::END_OF_CSV)
	if(data1[0] == "1234567")
	if(data1[1] == "2")
	if(data1[2] == "3123")
	if(data2[0] == "4")
	if (data2[1] == "5")
	if (data2[2] == "6")
	if (data3[0] == "")
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

bool test12() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
	CsvStreamNS::Ret ret1,ret2,ret3;

	if (!csv1.is_open()) {
		return false;
	}

	std::string data1[5];
	std::string data2[5];
	std::string data3[5];


	ret1 = csv1.readCells(data1,5);
	ret2 = csv1.readCells(data2,5);
	ret3 = csv1.readCells(data3,5);

	if(ret1 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret2 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret3 == CsvStreamNS::Ret::END_OF_CSV)
	if(data1[0] == "1234567")
	if(data1[1] == "2")
	if(data1[2] == "3123")
	if(data2[0] == "4")
	if (data2[1] == "5")
	if (data2[2] == "6")
	if (data3[0] == "")
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

bool test13() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[0]);
	CsvStreamNS::Ret ret1,ret2,ret3;

	if (!csv1.is_open()) {
		return false;
	}

	char data1[5][100];
	char data2[5][100];
	char data3[5][100];


	ret1 = csv1.readCells_c_str(&data1[0][0],5,100);
	ret2 = csv1.readCells_c_str(&data2[0][0],5,100);
	ret3 = csv1.readCells_c_str(&data3[0][0],5,100);

	if(ret1 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret2 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret3 == CsvStreamNS::Ret::END_OF_CSV)
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

template<class T> bool test14() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[1], std::ios_base::in | std::ios_base::out, true, &std::cout);
	CsvStreamNS::Ret ret;

	if (!csv1.is_open()) {
		return false;
	}

	int i;
	T data[20];
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

	if(data[0] == 1234567)
	if(data[1] == 2)
	if(data[2] == 3123)
	if(data[3] == 4)
	if (data[4] == 5)
	if (data[5] == 6)
	if (data[6] == 0)
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

template<class T> bool test15() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[1], std::ios_base::in | std::ios_base::out, true, &std::cout);
	CsvStreamNS::Ret ret1,ret2,ret3;

	if (!csv1.is_open()) {
		return false;
	}

	std::vector<T> data1;
	std::vector<T> data2;
	std::vector<T> data3;

	ret1 = csv1.readCells(data1);
	ret2 = csv1.readCells(data2);
	ret3 = csv1.readCells(data3);

	if(ret1 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret2 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret3 == CsvStreamNS::Ret::ERR)
	if(data1[0] == 1234567)
	if(data1[1] == 2)
	if(data1[2] == 3123)
	if(data2[0] == 4)
	if (data2[1] == 5)
	if (data2[2] == 6)
	if (data3[0] == 0)
	{
		csv1.close(); 
		return true;
	}

	csv1.close();
	return false;

}

template<class T> bool test16() {
	CsvStreamNS::basic_CsvStream<char> csv1(testdatas[1], std::ios_base::in | std::ios_base::out, true, &std::cout);
	CsvStreamNS::Ret ret1,ret2,ret3;

	if (!csv1.is_open()) {
		return false;
	}

	T data1[10];
	T data2[10];
	T data3[10];

	ret1 = csv1.readCells(data1,10);
	ret2 = csv1.readCells(data2,10);
	ret3 = csv1.readCells(data3,10);

	if(ret1 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret2 == CsvStreamNS::Ret::END_OF_ROW)
	if (ret3 == CsvStreamNS::Ret::ERR)
	if(data1[0] == 1234567)
	if(data1[1] == 2)
	if(data1[2] == 3123)
	if(data2[0] == 4)
	if (data2[1] == 5)
	if (data2[2] == 6)
	if (data3[0] == 0)
	{
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
	//EXECUTE_TEST(test7());
	EXECUTE_TEST(test8());
	EXECUTE_TEST(test9());
	EXECUTE_TEST(testDeleteFirstSpace1());
	EXECUTE_TEST(testDeleteFirstSpace2());
	EXECUTE_TEST(testDeleteLastSpace1());
	EXECUTE_TEST(testDeleteLastSpace2());
	EXECUTE_TEST(testDeleteSpace1());
	EXECUTE_TEST(testDeleteSpace2());
	EXECUTE_TEST(test10());
	EXECUTE_TEST(test11());
	EXECUTE_TEST(test12());
	EXECUTE_TEST(test13());
	EXECUTE_TEST(test14<int>());
	EXECUTE_TEST(test14<long>());
	EXECUTE_TEST(test14<long long>());
	EXECUTE_TEST(test14<unsigned long>());
	EXECUTE_TEST(test14<unsigned long long>());
	EXECUTE_TEST(test14<float>());
	EXECUTE_TEST(test14<double>());
	EXECUTE_TEST(test14<long double>());

	EXECUTE_TEST(test15<int>());
	EXECUTE_TEST(test15<long>());
	EXECUTE_TEST(test15<long long>());
	EXECUTE_TEST(test15<unsigned long>());
	EXECUTE_TEST(test15<unsigned long long>());
	EXECUTE_TEST(test15<float>());
	EXECUTE_TEST(test15<double>());
	EXECUTE_TEST(test15<long double>());

	EXECUTE_TEST(test16<int>());
	EXECUTE_TEST(test16<long>());
	EXECUTE_TEST(test16<long long>());
	EXECUTE_TEST(test16<unsigned long>());
	EXECUTE_TEST(test16<unsigned long long>());
	EXECUTE_TEST(test16<float>());
	EXECUTE_TEST(test16<double>());
	EXECUTE_TEST(test16<long double>());
	return 0;
}