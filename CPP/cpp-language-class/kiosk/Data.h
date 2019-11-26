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
	Material material[20]; // 재료 (원본)
	vector<Material> materialCategory; // 재료 카테고리
	/*
	[0]~[7] 버거
	[8]~[11] 사이드
	[12]~[15] 드링크
	[16]~[19] 디저트
	[20]~[23] 추가 재료
	*/
	vector<Product>* categoryArray[5]; // 모든 카테고리들의 포인터들을 담아놓은 배열
	vector<Product> burgerCategory; // *categoryArray[0]
	vector<Product> sideCategory; // *categoryArray[1]
	vector<Product> dessertCategory; // *categoryArray[2]
	vector<Product> drinkCategory; // *categoryArray[3]
	vector<Product> addmaterialCategory; // *categoryArray[4]

	vector<Product> currentCart; // 현재 사이클의 카트 하나
	vector<vector<Product>> cartList; // 현재 담겨 있는 모든 카트
	vector<vector<Product>> soldCartList; // 팔린 카트 리스트
	
	int for_here; // 매장 주문 횟수 포인터
	int to_go; // 포장 주문 횟수 포인터
	int total_sales; // 매출액

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
	void sales_update(); // 매출액 업데이트
	int get_total_sales();
};