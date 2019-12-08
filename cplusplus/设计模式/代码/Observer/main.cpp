#include "Subject.h"
#include "Observer.h"
#include <iostream>
using namespace std;
int main(int argc,char* argv[])
{
ConcreteSubject *sub=new ConcreteSubject();
Observer* o1=new ConcreteObserver(sub);

sub->SetState("old");
sub->notifyObserver();

sub->SetState("new");
sub->notifyObserver();

}

