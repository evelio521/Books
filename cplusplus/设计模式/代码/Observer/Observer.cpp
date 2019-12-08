#include "Observer.h"
#include "Subject.h"
#include <iostream>
#include <string>
using namespace std;
Observer::Observer()
{
	 _st='\0';

}

Observer::~Observer()
{
}
ConcreteObserver::ConcreteObserver(Subject *sub)

{
	
	_sb=sub;
	_sb->regiterObserver(this);
}

ConcreteObserver::~ConcreteObserver()

{
	_sb->removerObserver(this);
	if(_sb!=NULL)
		delete _sb;

}
Subject* ConcreteObserver::GetSubject()
{
	return _sb;
}
void ConcreteObserver::Update(Subject *sb)
{
	_st=sb->getState();
	PrintInfo();
}
void ConcreteObserver::PrintInfo()
{
	cout<<_sb->getState()<<endl;
}
