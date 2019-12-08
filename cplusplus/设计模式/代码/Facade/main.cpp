
#include "Facade.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])
{
Facade* f = new Facade();
f->operationWrapper();
return 0;
}