#include "FileManager.h"

void FileManager::open(string fileName) {
	reader.open(fileName);
}

/*우영*/
void FileManager::addProduct(string fileName, string itemName, int* materialList, int size)
{
	reader.open(fileName, ios::app);
	string fileInput = "\n"+itemName;
	int i;
	for (i = 0; i < size; i++)
		fileInput += "_" + to_string(materialList[i]);
	write(fileName, fileInput);
	close();
}
void FileManager::deleteProduct(string fileName, string deletedContent)
{
	int contentLineNum = findContentLine(fileName, deletedContent); //지우기 원하는 content의 lineNum을 contentLineNum에 저장
	cout << contentLineNum << endl;
	int fileLineNum = countLines(fileName); //file의 lineNum
	int i;
	string writeLine = ""; //writeLine : 수정된 파일 내용을 담을 string
	string *targetLine = tokenize(fileName, contentLineNum, 5); //수정되는 line을 tokenize하여 저장하는 객체
	string tempLine; //파일을 tempLine에 한줄씩 담아 writeLine에 옮김
	size_t pos;
	open(fileName);
	if (contentLineNum == -1)
	{
		cout << "바꾸려는 재료가 존재하지 않습니다." << endl;
		close();
		return;
	}
	for (i = 0; i < contentLineNum - 1; i++)
	{
		getline(reader, tempLine);
		writeLine += tempLine;
		writeLine += '\n';
	}
	getline(reader, tempLine);
	for (i = contentLineNum; i < fileLineNum; i++)
	{
		getline(reader, tempLine);
		writeLine += tempLine;
		if (i != fileLineNum - 1)
			writeLine += '\n';
	}
	reader.close();
	ofstream writeFile(fileName, ios::trunc);
	writeFile << writeLine;
}
void FileManager::modifyPrice(string fileName, string content, int modifiedPrice)
{
	int contentLineNum = findContentLine(fileName, content); //바꾸기 원하는 content의 lineNum을 contentLineNum에 저장
	int fileLineNum = countLines(fileName); //file의 lineNum
	int i;
	string writeLine = ""; //writeLine : 수정된 파일 내용을 담을 string
	string* targetLine = tokenize(fileName, contentLineNum, 5); //수정되는 line을 tokenize하여 저장하는 객체
	string tempLine; //파일을 tempLine에 한줄씩 담아 writeLine에 옮김
	size_t pos;
	targetLine[2] = to_string(modifiedPrice);
	open(fileName);
	if (contentLineNum == -1)
	{
		close();
		return;
	}
	for (i = 0; i < contentLineNum - 1; i++)
	{
		getline(reader, tempLine);
		writeLine += tempLine;
		writeLine += '\n';
	}
	getline(reader, tempLine);
	writeLine += targetLine[0] + "_" + targetLine[1] + "_" + targetLine[2] + "_" + targetLine[3] + "\n";
	for (i = contentLineNum; i < fileLineNum; i++)
	{
		getline(reader, tempLine);
		writeLine += tempLine;
		if (i != fileLineNum - 1)
			writeLine += '\n';
	}
	reader.close();
	ofstream writeFile(fileName, ios::trunc);
	writeFile << writeLine;
}
void FileManager::modifyStock(string fileName, string content, int modifiedContent)
{
	int contentLineNum = findContentLine(fileName, content); //바꾸기 원하는 content의 lineNum을 contentLineNum에 저장
	int fileLineNum = countLines(fileName); //file의 lineNum
	int i;
	string writeLine = ""; //writeLine : 수정된 파일 내용을 담을 string
	string *targetLine = tokenize(fileName, contentLineNum, 5); //수정되는 line을 tokenize하여 저장하는 객체
	string tempLine; //파일을 tempLine에 한줄씩 담아 writeLine에 옮김
	size_t pos;
	targetLine[1] = to_string(modifiedContent);
	open(fileName);
	if (contentLineNum == -1)
	{
		close();
		return;
	}
	for (i = 0; i < contentLineNum-1; i++)
	{
		getline(reader, tempLine);
		writeLine += tempLine;
		writeLine += '\n';
	}
	getline(reader, tempLine);
	writeLine += targetLine[0] + "_" + targetLine[1] + "_" + targetLine[2] + "_" + targetLine[3] + "\n";
	for (i = contentLineNum; i < fileLineNum; i++)
	{
		getline(reader, tempLine);
		writeLine += tempLine;
		if (i != fileLineNum - 1)
			writeLine += '\n';
	}
	reader.close();
	ofstream writeFile(fileName, ios::trunc);
	writeFile << writeLine;
}

int FileManager::findContentLine(string fileName, string content) //content와 동일한 이름이 있는 line의 위치 lineNum 반환
{
	int lineNum;
	int i;
	lineNum = countLines(fileName);
	for (i = 0; i < lineNum; i++) {
		if (content == tokenize(fileName, i+1, 4)[0]) //file을 한줄씩 읽으며 첫번째 요소(product name)와 content가 일치하는 경우, lineNum를 반환
			return i+1;
	}
	return -1;
}
/*우영*/
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