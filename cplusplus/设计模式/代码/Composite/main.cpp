#include "Component.h"
#include "Composite.h"
#include "Leaf.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])
{
	Leaf* l=new Leaf();
	l->operation();

	Composite* com=new Composite();
	com->Add(l);
	com->Add(l);
	com->Add(l);
	com->Add(l);
	com->Add(l);
	com->Add(l);
	com->Add(l);
	com->Add(l);
	com->Add(l);
	
	com->operation();
	com->Remove(l);
	com->operation();
	Component* ll = com->getChild(0);
	ll->operation();

}