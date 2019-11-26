#include "FileManager.h"

void FileManager::open(string fileName) {
	reader.open(fileName);
}

void FileManager::write(string fileName, string content) { // 파일에 content 내용을 씀 
	ofstream write(fileName, ios::app);

	write << content;
}

void FileManager::close() {
	reader.close();
}

void FileManager::jumpLine(int line) { // 현재 줄로부터 line 줄 아래로 이동
	string s;
	for (size_t i = 0; i < line; i++)
	{
		getline(reader, s);
	}
}

int FileManager::countLines(string fileName) { // 몇 줄 짜리 파일인지 return
	reader.open(fileName);
	int count = 0;
	string s;
	while (getline(reader, s)) ++count;
	reader.close();
	return count;
}

string* FileManager::tokenize(string fileName, int line, size_t array_size) { // 파일의 line번 째 줄을 tokenize
	reader.open(fileName);
	if (line <= 0) return 0;
	jumpLine(line - 1); // line 번째 줄로 이동
	string s = "";
	getline(reader, s); // 한 줄 읽기
	std::string delimiter = "_";
	size_t pos = 0;
	string* str_arr = new string[array_size];
	size_t i;
	for (i = 0; i < array_size; i++) {
		pos = s.find(delimiter);
		if (pos != -1) {
			str_arr[i] = s.substr(0, pos);
			s.erase(0, pos + 1);
			// cout << i << ":" << str_arr[i] << endl; // 디버깅
		}
		else {
			str_arr[i] = s;
			s = "";
			// cout << i << ":" << str_arr[i] << endl; // 디버깅
			for (size_t j = i + 1; j < array_size; j++) {
				str_arr[j] = "";
				// cout << j << ":" << str_arr[j] << endl; // 디버깅
			}
			break;
		}
	}
	reader.close();
	return str_arr; // tokenizing 된 string 배열
}

void FileManager::setTime(string fileName) {  // 시간을 text에  씀
	ofstream write(fileName, ios::app);

	time_t  seconds;

	time(&seconds);  // 

	struct tm timebuf;

	localtime_s(&timebuf, &seconds);

	int year = 1900 + timebuf.tm_year;
	int month = 1 + timebuf.tm_mon;
	int day = timebuf.tm_mday;
	int hour = timebuf.tm_hour;
	int minute = timebuf.tm_min;
	int second = timebuf.tm_sec;

	write << year << "/" << month << "/" << day << "/" << hour << "/" << minute << "/" << second;


}