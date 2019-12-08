#ifndef _OBSERVER_H_
#define _OBSERVER_H_
#include "Subject.h"
#include <string>
using namespace std;
typedef string State;

class Observer{

public:
	virtual ~ Observer();
	virtual void Update(Subject * sb)=0;
	virtual void PrintInfo()=0;
protected:
	 Observer();
	 State _st;

};

class ConcreteObserver:public Observer
{
public:
	virtual Subject* GetSubject();
	 ConcreteObserver(Subject *sub);
	 virtual ~ConcreteObserver();
	 void Update(Subject * sb);
	 void PrintInfo();
protected:
	Subject *_sb;
};
#endif