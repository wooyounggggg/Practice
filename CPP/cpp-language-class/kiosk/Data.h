#pragma once
#include <iostream>
#include "Product.h"
#include "FileManager.h"
using namespace std;

#define BREAD 0
#define CHEESE 1
#define TOMATO 2
#define LETTUCE 3
#define SOURCE 4
#define MEAT 5
#define SHRIMP 6
#define CHICKEN 7
#define POTATO 8
#define CORN 9
#define BEAN 10
#define WATER 11
#define MILK 12
#define ICECREAM 13
#define MUSHROOM 14
#define BACON 15
#define JALAPENO 16
#define CHOCO 17
#define COKE 18
#define CIDER 19

class Data {
private:
	FileManager* fm;
	Material material[20]; // ��� (����)
	vector<Material> materialCategory; // ��� ī�װ�
	/*
	[0]~[7] ����
	[8]~[11] ���̵�
	[12]~[15] �帵ũ
	[16]~[19] ����Ʈ
	[20]~[23] �߰� ���
	*/
	vector<Product>* categoryArray[5]; // ��� ī�װ����� �����͵��� ��Ƴ��� �迭
	vector<Product> burgerCategory; // *categoryArray[0]
	vector<Product> sideCategory; // *categoryArray[1]
	vector<Product> dessertCategory; // *categoryArray[2]
	vector<Product> drinkCategory; // *categoryArray[3]
	vector<Product> addmaterialCategory; // *categoryArray[4]

	vector<Product> currentCart; // ���� ����Ŭ�� īƮ �ϳ�
	vector<vector<Product>> cartList; // ���� ��� �ִ� ��� īƮ
	vector<vector<Product>> soldCartList; // �ȸ� īƮ ����Ʈ
	
	int for_here; // ���� �ֹ� Ƚ�� ������
	int to_go; // ���� �ֹ� Ƚ�� ������
	int total_sales; // �����

public:
	Data(FileManager* fm);
	void initalize();
	void addDefaultMaterial(vector<Material>& materialList, Material material[]);
	vector<Product>** getCategoryArray();
	vector<vector<Product>>& getCartList();
	vector<Product>& getCurrentCart();
	vector<Material>* getMaterialCategory();
	vector<vector<Product>>& getSoldCartList(); 
	int* get_for_here();
	int* get_to_go();
	void sales_update(); // ����� ������Ʈ
	int get_total_sales();
};