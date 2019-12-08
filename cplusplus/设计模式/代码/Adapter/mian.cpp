#include "Adapter.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])

{

	Adaptee *ade=new Adaptee();
	Target* adt=new Adapter();

	adt->Request();

}