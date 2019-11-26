#pragma once
#include <iostream>
#include <vector>
#include "Material.h"
using namespace std;

class Material;

class Product
{
private:
	string name;
	vector<Material> materialList;
	int *price;
	int kcal;

public:
	Product(string name, vector<Material> materialList);
	Product(const Product& p);
	void setName(string name);
	string getName();
	void setMaterialList(vector<Material> materialList);
	vector<Material> getMaterialList();
	int getPrice();
	int getKcal();
	void addKcal(Material m);
	int renewProduct();
	void addPrice(Material m);
	void addMaterial(Material m);
};