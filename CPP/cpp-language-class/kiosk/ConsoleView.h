#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "Product.h"
#include "Data.h"
#include "FileManager.h"
using namespace std;

class ConsoleView {
private:
	Data* data;
	FileManager* fm;
	char input = '0';
	int page = 0;
	boolean burger = false;
	boolean side = false;
	boolean drink = false;
	char comboAndSet;
	char option;// � ī�װ��� �����ߴ���
	int incresedCartSize = 0; // �Է½� �þ�� īƮ ������ üũ

public:
	ConsoleView(Data* data, FileManager* fm);
	void gotoxy(short x, short y);
	void showUI();
	int userUI0();
	int userUI1();
	int userUI2();
	int userUI3();
	int userUI4();
	int userUI5();
	int userUI6();
	int helpUI();
	int adminUI0();
	int adminUI1();
	int adminUI2();
	int adminUI3();
	int adminUI4();
	void print_all_elements(vector<Product>& list);
	void line_clear();
	void console_clear();
	void border_above();
	void border_bottom();
	void pay();
	void inputs();
};
