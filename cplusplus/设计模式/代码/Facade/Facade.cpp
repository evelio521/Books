#include "Facade.h"
#include <iostream>
using namespace std;

Subsystem1::Subsystem1()
{
}
Subsystem1::~Subsystem1()
{
}
void Subsystem1::Operation()
{
	cout<<"Subsystem1 operation"<<endl;
}

Subsystem2::Subsystem2()
{
}
Subsystem2::~Subsystem2()
{
}
void Subsystem2::Operation()
{
	cout<<"Subsystem2 operation"<<endl;
}

Facade::Facade()
{
	this->sub1=new Subsystem1();
	this->sub2=new Subsystem2();

}


Facade::~Facade()
{
	delete sub1;
	delete sub2;
}

void Facade::operationWrapper()
{
	sub1->Operation();
	sub2->Operation();
}