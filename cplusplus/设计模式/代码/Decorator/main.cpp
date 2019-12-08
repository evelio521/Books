#include "Decorator.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[]){
Component * com=new ConcreteComponent();
Decorator* dec=new ConcreteDecorator(com);
Decorator* dec2=new ConcreteDecorator2(com);

dec->operation();
dec2->operation();

delete dec;


}