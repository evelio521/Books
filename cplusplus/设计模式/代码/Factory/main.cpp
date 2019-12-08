#include "Factory.h"
#include "Product.h"

#include <iostream>
using namespace std;

int main(int argc,char* argv[])
{

	Factory *fac=new ConcreteFactory();
	Product* pro=fac->CreateProduct("ConcreteProduct2");
return 0;
}