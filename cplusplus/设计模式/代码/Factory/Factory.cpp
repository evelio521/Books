#include "Factory.h"
#include "Product.h"
#include <iostream>
using namespace std;

Factory::Factory()
{
}
Factory::~Factory()
{
}
Product* ConcreteFactory::CreateProduct(char* str)
{
	if(str=="ConcreteFactory")
	return new ConcreteProduct();
	else 
	return new ConcreteProduct2();
}

ConcreteFactory::ConcreteFactory(){cout<<"ConcreteFactory..."<<endl;}
ConcreteFactory::~ConcreteFactory(){}