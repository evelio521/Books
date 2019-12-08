#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class Component
{
public:
	Component();
	virtual ~Component();
	virtual void operation()=0;
	virtual void Add(const Component&);
	virtual void Remove(const Component&);
	virtual Component* getChild(int);

};
#endif
