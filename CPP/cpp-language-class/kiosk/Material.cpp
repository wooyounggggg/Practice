#include "Material.h"

Material::Material() {
	this->name = "";
	this->kcal = 0;
	this->stock = new int(1);
	this->price = new int(1);
}

Material::Material(string name, int stock, int price, int kcal)
{
	this->name = name;
	this->kcal = kcal;
	this->price = new int(1);
	this->stock = new int(1);
	*this->price = price;
	*this->stock = stock;
}

Material::Material(const Material& m){
	this->name = m.name;
	this->kcal = m.kcal;
	this->stock = m.stock;
	this->price = m.price;
	
}

int Material::getStock()
{
	return *this->stock;
}
string Material::getName()
{
	return this->name;
}
int Material::getPrice()
{
	return *this->price;
}
int Material::getKcal()
{
	return this->kcal;
}
/*우영*/
int Material::increaseStock(int stock) {
	*this->stock += stock;
	return *this->stock;
}
void Material::increasePrice(int price) {
	*this->price += price;
}

int Material::decreaseStock(int stock) {
	*this->stock -= stock;
	return *this->stock;
}
/*우영*/