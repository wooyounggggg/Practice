#include "FileManager.h"

void FileManager::open(string fileName) {
	reader.open(fileName);
}

void FileManager::write(string fileName, string content) { // ���Ͽ� content ������ �� 
	ofstream write(fileName, ios::app);

	write << content;
}

void FileManager::close() {
	reader.close();
}

void FileManager::jumpLine(int line) { // ���� �ٷκ��� line �� �Ʒ��� �̵�
	string s;
	for (size_t i = 0; i < line; i++)
	{
		getline(reader, s);
	}
}

int FileManager::countLines(string fileName) { // �� �� ¥�� �������� return
	reader.open(fileName);
	int count = 0;
	string s;
	while (getline(reader, s)) ++count;
	reader.close();
	return count;
}

string* FileManager::tokenize(string fileName, int line, size_t array_size) { // ������ line�� ° ���� tokenize
	reader.open(fileName);
	if (line <= 0) return 0;
	jumpLine(line - 1); // line ��° �ٷ� �̵�
	string s = "";
	getline(reader, s); // �� �� �б�
	std::string delimiter = "_";
	size_t pos = 0;
	string* str_arr = new string[array_size];
	size_t i;
	for (i = 0; i < array_size; i++) {
		pos = s.find(delimiter);
		if (pos != -1) {
			str_arr[i] = s.substr(0, pos);
			s.erase(0, pos + 1);
			// cout << i << ":" << str_arr[i] << endl; // �����
		}
		else {
			str_arr[i] = s;
			s = "";
			// cout << i << ":" << str_arr[i] << endl; // �����
			for (size_t j = i + 1; j < array_size; j++) {
				str_arr[j] = "";
				// cout << j << ":" << str_arr[j] << endl; // �����
			}
			break;
		}
	}
	reader.close();
	return str_arr; // tokenizing �� string �迭
}

void FileManager::setTime(string fileName) {  // �ð��� text��  ��
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