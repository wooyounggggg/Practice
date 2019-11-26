#include "Data.h"
#include "Material.h"

Data::Data(FileManager* fm) {
	this->fm = fm;
	initalize();
	for_here = 0;
	to_go = 0;
	total_sales = 0;
}

void Data::initalize(){
	// 재료 초기화
	Material material[20];
	for (size_t i = 0; i < 20; i++)
	{
		string* s = fm->tokenize("material.txt", i+1, 4);
		material[i] = Material(s[0], atoi(s[1].c_str()), atoi(s[2].c_str()), atoi(s[3].c_str()));
	}
	
	// 재료 카테고리 초기화
	for (int i = 0; i < 20; i++)
	{
		materialCategory.push_back(material[i]);
	}

	// 제품 초기화 및 카테고리 초기화
	string fileName[5] = { "product_burger.txt", "product_side.txt", "product_drink.txt", "product_dessert.txt", "product_extra_material.txt" };
	for (size_t n = 0; n < 5; n++)
	{
		for (size_t i = 0; i < fm->countLines(fileName[n]); i++)
		{
			string* s = fm->tokenize(fileName[n], i + 1, 10); // 최대 10개의 재료까지 커버 가능
			vector<Material> materialList;
			if (n == 0) addDefaultMaterial(materialList, material); // 버거만 default 재료 추가
			int j = 1;
			while (s[j] != "") {
				materialList.push_back(material[atoi(s[j].c_str())]);
				j++;
			}
			Product p(s[0], materialList);
			switch (n) {
			case 0:
				burgerCategory.push_back(p);
				break;
			case 1:
				sideCategory.push_back(p);
				break;
			case 2:
				drinkCategory.push_back(p);
				break;
			case 3:
				dessertCategory.push_back(p);
				break;
			case 4:
				addmaterialCategory.push_back(p);
				break;
			}
		}
	}

	// categoryArray 초기화
	for (unsigned int i = 0; i < 5; i++)
	{
		categoryArray[0] = &burgerCategory;
		categoryArray[1] = &sideCategory;
		categoryArray[2] = &dessertCategory;
		categoryArray[3] = &drinkCategory;
		categoryArray[4] = &addmaterialCategory;
	}
}

void Data::addDefaultMaterial(vector<Material>& materialList, Material material[])
{
	materialList.push_back(material[BREAD]);
	materialList.push_back(material[LETTUCE]);
	materialList.push_back(material[SOURCE]);
	materialList.push_back(material[TOMATO]);
}

vector<Product>** Data::getCategoryArray() {
	return categoryArray;
}

vector<vector<Product>>& Data::getCartList() {
	return cartList;
}

vector<Product>& Data::getCurrentCart() {
	return currentCart;
}

vector<Material>* Data::getMaterialCategory(){
	return &materialCategory;
}

vector<vector<Product>>& Data::getSoldCartList() {
	return soldCartList;
}

int* Data::get_for_here() {
	return &for_here;
}

int* Data::get_to_go() {
	return &to_go;
}

void Data::sales_update() { // 매출액 업데이트
	int sum = 0;
	for (size_t i = 0; i < this->soldCartList.size(); i++)
	{
		for (size_t j = 0; j < soldCartList.at(i).size(); j++)
		{
			sum += soldCartList.at(i).at(j).getPrice();
		}
	}
	this->total_sales = sum;
}
int Data::get_total_sales() {
	return this->total_sales;
}