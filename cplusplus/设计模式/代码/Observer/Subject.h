#ifndef _SUBJECT_H_
#define _SUBJECT_H_
#include <list>
#include <string>
using namespace std;
typedef string State;

class Observer;
class Subject{

public:
	virtual void regiterObserver(Observer *ob);
	virtual void removerObserver(Observer *ob);
	virtual void notifyObserver();
	virtual State getState()=0; 
	virtual void SetState(const State& sd)=0;
	virtual ~Subject();
protected:
	Subject();
private:
	list<Observer*> * _ob;

};
class ConcreteSubject:public Subject

{
public:
	ConcreteSubject();
	~ConcreteSubject();
	 State getState(); 
	 void SetState(const State& sd);

private:
	State _st;

};
#endif