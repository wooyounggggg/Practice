#include "FileManager.h"

void FileManager::open(string fileName) {
	reader.open(fileName);
}

/*�쿵*/
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
	int contentLineNum = findContentLine(fileName, deletedContent); //����� ���ϴ� content�� lineNum�� contentLineNum�� ����
	cout << contentLineNum << endl;
	int fileLineNum = countLines(fileName); //file�� lineNum
	int i;
	string writeLine = ""; //writeLine : ������ ���� ������ ���� string
	string *targetLine = tokenize(fileName, contentLineNum, 5); //�����Ǵ� line�� tokenize�Ͽ� �����ϴ� ��ü
	string tempLine; //������ tempLine�� ���پ� ��� writeLine�� �ű�
	size_t pos;
	open(fileName);
	if (contentLineNum == -1)
	{
		cout << "�ٲٷ��� ��ᰡ �������� �ʽ��ϴ�." << endl;
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
	int contentLineNum = findContentLine(fileName, content); //�ٲٱ� ���ϴ� content�� lineNum�� contentLineNum�� ����
	int fileLineNum = countLines(fileName); //file�� lineNum
	int i;
	string writeLine = ""; //writeLine : ������ ���� ������ ���� string
	string* targetLine = tokenize(fileName, contentLineNum, 5); //�����Ǵ� line�� tokenize�Ͽ� �����ϴ� ��ü
	string tempLine; //������ tempLine�� ���پ� ��� writeLine�� �ű�
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
	int contentLineNum = findContentLine(fileName, content); //�ٲٱ� ���ϴ� content�� lineNum�� contentLineNum�� ����
	int fileLineNum = countLines(fileName); //file�� lineNum
	int i;
	string writeLine = ""; //writeLine : ������ ���� ������ ���� string
	string *targetLine = tokenize(fileName, contentLineNum, 5); //�����Ǵ� line�� tokenize�Ͽ� �����ϴ� ��ü
	string tempLine; //������ tempLine�� ���پ� ��� writeLine�� �ű�
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

int FileManager::findContentLine(string fileName, string content) //content�� ������ �̸��� �ִ� line�� ��ġ lineNum ��ȯ
{
	int lineNum;
	int i;
	lineNum = countLines(fileName);
	for (i = 0; i < lineNum; i++) {
		if (content == tokenize(fileName, i+1, 4)[0]) //file�� ���پ� ������ ù��° ���(product name)�� content�� ��ġ�ϴ� ���, lineNum�� ��ȯ
			return i+1;
	}
	return -1;
}
/*�쿵*/
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