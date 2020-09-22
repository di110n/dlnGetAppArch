// dlnGetAppArch.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdexcept>

using namespace std;

typedef unsigned char byte;

bool fileIsExist(string filePath);
double getFileSize(string filePath);
bool checkApp(string filePath);
bool get3c(string filePath, byte *b3c);
void getSignature(string filePath, byte b3c, byte *signature);
string gaa(byte signature[2]);

int main(int argc, char* argv[])
{
	if (argc != 2) {
		cout << "\nUsage: dlngaa <application filename>\n\n";
		exit(1);
	}

	string path = argv[1];
	
	if (!fileIsExist(path)) {
		cout << "No such file.";
		exit(2);
	}
	if (getFileSize(path) < 0xff) {
		cout << "Invalid application size.";
		exit(3);
	}
	
	if (!checkApp(path)) {
		cout << "File is not application.";
		exit(7);
	}
	
	byte b3c = 0x0;
	byte *pb3c = &b3c;
	
	if (!get3c(path, pb3c)) {
		cout << "Invalid application.";
		exit(4);
	}

	byte signature[2] = { 0x0, 0x0 };
	byte *psig = &signature[0];
	
	getSignature(path, b3c, psig);

	string result = gaa(signature);

	cout << result.c_str();
}

bool fileIsExist(string filePath)
{
	bool isExist = false;
	ifstream fin(filePath.c_str());
	if (fin.is_open())
		isExist = true;

	fin.close();
	return isExist;
}

double getFileSize(string filePath) {
	struct stat stat_buf;
	double rc = stat(filePath.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

bool checkApp(string filePath) {
	FILE *fin;
	byte tmp[3] = { 0x0, 0x0, 0x0 };
	unsigned int v = 0x0;
	int tmpl = sizeof(tmp);

	if (fopen_s(&fin, filePath.c_str(), "rb") == 0) {
		fread_s(&tmp, tmpl, 1, tmpl, fin);
		fclose(fin);
		v = tmp[0];
		v = (v << 8) | tmp[1];
		v = (v << 8) | tmp[2];
	}
	else {
		cout << "Error while reading the file.";
		exit(6);
	}

	if (v != 0x4d5a90) {
		return false;
	}
	return true;
}

bool get3c(string filePath, byte *b3c) {
	FILE *fin;
	byte tmp = 0x0;
	int tmpl = sizeof(tmp);
	
	if (fopen_s(&fin, filePath.c_str(), "rb") == 0) {
		fseek(fin, 0x3c, 0);
		fread_s(&tmp, tmpl, 1, tmpl, fin);
		fclose(fin);
	}
	else {
		cout << "Error while reading the file.";
		exit(4);
	}
	
	if ((tmp > 0x3c) and (tmp < 0xff)) {
		*b3c = tmp;
		return true;
	}
	return false;
}

void getSignature(string filePath, byte b3c, byte *signature) {
	FILE *fin;
	byte tmp[2] = { 0x0, 0x0 };
	int tmpl = sizeof(tmp);
	
	if (fopen_s(&fin, filePath.c_str(), "rb") == 0) {
		fseek(fin, b3c+4, 0);
		fread_s(&tmp, tmpl, 1, tmpl, fin);
		fclose(fin);
		signature[0] = tmp[1];
		signature[1] = tmp[0];
	}
	else {
		cout << "Error while reading the signature.";
		exit(5);
	}
}

string gaa(byte signature[2]) {
	unsigned short v = signature[0];
	v = (v << 8) | signature[1];

	switch (v) {
		case 0x14c:
			return "I386";
		case 0x8664:
			return "AMD64";
		case 0x1d3:
			return "Matsushita AM33";
		case 0x1c0:
			return "ARM";
		case 0xaa64:
			return "ARM64";
		case 0x1c4:
			return "ARM Thumb-2";
		case 0xebc:
			return "EFI byte code";
		case 0x200:
			return "IA64";
		case 0x9041:
			return "Mitsubishi M32R";
		case 0x266:
			return "MIPS16";
		case 0x366:
			return "MIPSFPU";
		case 0x466:
			return "MIPSFPU16";
		case 0x1f0:
			return "POWERPC";
		case 0x1f1:
			return "POWERPCFP";
		case 0x166:
			return "MIPS";
		case 0x5032:
			return "RISC-V32";
		case 0x5064:
			return "RISC-V64";
		case 0x5128:
			return "RISC-V128";
		case 0x1a2:
			return "Hitachi SH3";
		case 0x1a3:
			return "Hitachi SH3 DSP";
		case 0x1a6:
			return "Hitachi SH4";
		case 0x1a8:
			return "Hitachi SH5";
		case 0x1c2:
			return "Thumb";
		case 0x169:
			return "MIPS WCE v2";
		default:
			return "Unknown architecture";
	}

	return "Unknown architecture";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
