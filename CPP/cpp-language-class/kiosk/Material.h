#pragma once
#include <iostream>
#include <vector>
#include "Product.h"
using namespace std;

class Product;

class Material
{
private:
	string name;
	int kcal;
	int* stock;
	int* price;

public:
	Material();
	Material(string name, int stock, int price, int kcal);
	Material(const Material& m);
	int getStock();
	string getName();
	int getPrice();
	int getKcal();
	int increaseStock(int stock);
	void increasePrice(int price);
	int decreaseStock(int stock);
};