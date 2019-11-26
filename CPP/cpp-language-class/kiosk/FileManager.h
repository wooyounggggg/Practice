#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class FileManager {
private:
	ifstream reader;

public:
	void open(string fileName);
	void deleteProduct(string fileName, string content);
	void modifyStock(string fileName, string content, int modifiedContent);
	int findContentLine(string fileName, string content);
	void write(string fileName, string content);
	void close();
	int countLines(string fileName);
	string* tokenize(string fileName, int line, size_t array_size);
	void jumpLine(int lineNum);
	void setTime(string fileName);
};