#ifndef _ADAPTER_H_
#define _ADAPTER_H_


class Target{
public:
	Target();
	virtual ~Target();
	virtual void Request();


};

class Adaptee
{
public:
	Adaptee();
	~Adaptee();
	void specialRequest();

};

class Adapter:public Target,private Adaptee
{

public:
	Adapter();
	~Adapter();
	void Request();

};
#endif