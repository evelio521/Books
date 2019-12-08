#include "Template.h"
#include <iostream>
using namespace std;

AbstractClass::AbstractClass()
{
}
AbstractClass::~AbstractClass()
{

}
void AbstractClass::TemplateMethod()
{
	this->PromitivveOperation1();
	this->PromitivveOperation2();
}


ConcreteClass1::ConcreteClass1()
{
}
ConcreteClass1::~ConcreteClass1()
{
}

void ConcreteClass1::PromitivveOperation1()
{
	cout<<"ConcreteClass1...PrimitiveOperation1"<<endl;
}
void ConcreteClass1::PromitivveOperation2()

{
	cout<<"ConcreteClass1...PrimitiveOperation2"<<endl;
}


ConcreteClass2::ConcreteClass2()
{
}
ConcreteClass2::~ConcreteClass2()
{
}
void ConcreteClass2::PromitivveOperation1()
{
cout<<"ConcreteClass2...PrimitiveOperation1"<<endl;
}
void ConcreteClass2::PromitivveOperation2()
{
cout<<"ConcreteClass2...PrimitiveOperation2"<<endl;
}