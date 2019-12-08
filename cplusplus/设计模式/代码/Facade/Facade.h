#ifndef _FACADE_H_
#define _FACADE_H_

class Subsystem1{
public:
	Subsystem1();
	~Subsystem1();
	void Operation();


};
class Subsystem2{
public:
	Subsystem2();
	~Subsystem2();
	void Operation();

};
class Facade{

public:
	Facade();
	~Facade();
	void  operationWrapper();
private:
	Subsystem1 * sub1;
	Subsystem2 * sub2;
};

#endif