#include "Decorator.h"
#include <iostream>
using namespace std;
Component::Component(){}
Component::~Component(){}

void Component::operation(){}

ConcreteComponent::ConcreteComponent()
{
}
ConcreteComponent::~ConcreteComponent(){}

void ConcreteComponent::operation()
{
	cout<<"ConcreteCpmponent"<<endl;
}

Decorator::Decorator(Component *_com)
{
	this->com=_com;

}

Decorator::~Decorator()
{
	delete com;
}
void Decorator::operation()
{
	

}

ConcreteDecorator::ConcreteDecorator(Component *_com):Decorator(_com)
{
	
}
ConcreteDecorator::~ConcreteDecorator(){}
void ConcreteDecorator::addNew()
{
	cout<<"addnew"<<endl;
}
void ConcreteDecorator::operation()
{
	com->operation();
	this->addNew();
}


ConcreteDecorator2::ConcreteDecorator2(Component *_com):Decorator(_com)
{
	
}
ConcreteDecorator2::~ConcreteDecorator2(){}
void ConcreteDecorator2::addNew()
{
	cout<<"addnew2"<<endl;
}
void ConcreteDecorator2::operation()
{
	com->operation();
	this->addNew();
}