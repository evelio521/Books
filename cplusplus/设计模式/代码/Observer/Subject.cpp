#include "Subject.h"
#include "Observer.h"
#include <iostream>
#include <list>
using namespace std;
typedef string State;

Subject::Subject()
{
	 _ob=new list<Observer*>;
}
Subject::~Subject()
{

}

void Subject::regiterObserver(Observer *ob){

	_ob->push_front(ob);
}
void Subject::removerObserver(Observer *ob)
{
	if(ob!=NULL)
		_ob->remove(ob);
}
void Subject::notifyObserver()
{

	list<Observer*>::iterator it;
	it=_ob->begin();

	for(;it!=_ob->end();++it)
	{
		(*it)->Update(this);
	}
}
ConcreteSubject::ConcreteSubject()
{
	_st='\0';
}

void ConcreteSubject::SetState(const State &sd)
{
	_st=sd;
}
State ConcreteSubject::getState()

{
	return _st;
}
ConcreteSubject::~ConcreteSubject(){}