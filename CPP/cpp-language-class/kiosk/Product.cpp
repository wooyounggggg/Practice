#include "Product.h"

Product::Product(string name, vector<Material> materialList) 
{
	this->name = name;
	this->materialList = materialList;
	this->price = new int(1);
	int size = materialList.size();
	*price = kcal = 0;
	for (int i = 0; i < size; i++) // 제품에 들어가는 재료를 탐색하여 제품의 가격과 칼로리 계산
	{
		*price += materialList.at(i).getPrice();
		kcal += materialList.at(i).getKcal();
	}
}
Product::Product(const Product& p) {
	this->name = p.name;
	this->materialList = p.materialList;
	this->price = p.price;
	this->kcal = p.kcal;
}

void Product::setName(string name)
{
	this->name = name;
}
string Product::getName()
{
	return this->name;
}
void Product::setMaterialList(vector<Material> materialList)
{
	this->materialList = materialList;
}
vector<Material> Product::getMaterialList()
{
	return this->materialList;
}
int Product::getPrice()
{
	return renewProduct();
}

int Product::getKcal()
{
	return this->kcal;
}
void Product::addKcal(Material m)
{
	this->kcal += m.getKcal();
}

int Product::renewProduct()
{
	int i, size;
	size = materialList.size();
	*this->price = 0;
	for (i = 0; i < size; i++)
		*this->price += materialList.at(i).getPrice();
	return *this->price;
}
void Product::addPrice(Material m)
{
	*this->price += m.getPrice();
}
void Product::addMaterial(Material m)
{
	materialList.push_back(m);
	addPrice(m);
	addKcal(m);
}