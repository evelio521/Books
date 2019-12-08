#include "Singleton.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])
{
	Singleton* sgn=Singleton::Instance();
	Singleton* sgn2=Singleton::Instance();
	return 0;


}