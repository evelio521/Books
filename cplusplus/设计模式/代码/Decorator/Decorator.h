#ifndef _DECORATOR_H_
#define _DECORATOR_H_

class Component{
public:
	virtual~ Component();
	virtual void operation();
protected:
	 Component();

};

class ConcreteComponent:public  Component{
public:

	 ConcreteComponent();
	 ~ ConcreteComponent();
	 void operation();



};

class Decorator:public Component{

public:
	virtual ~Decorator();
	virtual void operation();
	Decorator(Component* _com);
protected:
	Component* com;
};

class ConcreteDecorator:public Decorator
{
public:
	ConcreteDecorator(Component* _com);
	~ConcreteDecorator();
	void operation();
	void addNew();


};
class ConcreteDecorator2:public Decorator
{
public:
	ConcreteDecorator2(Component* _com);
	~ConcreteDecorator2();
	void operation();
	void addNew();


};

#endif
