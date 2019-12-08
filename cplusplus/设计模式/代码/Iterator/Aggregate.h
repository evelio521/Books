#ifndef _AGGREGATE_H_
#define _AGGREGATE_H_


typedef int Object;
class Iterator;

class Aggregate{

public:
	virtual ~Aggregate();
	virtual Iterator* CreateIterator()=0;
	virtual Object GetItem(int idx)=0;

	virtual int GetSize()=0;
protected:
	Aggregate();

};
class ConcreteAggregate:public Aggregate{
public:
	enum{SZIE=3};
	 ConcreteAggregate();
	 ~ ConcreteAggregate();
	 Iterator* CreateIterator();
	 Object GetItem(int idx);
	 int GetSize();
private:
	Object _objs[SZIE];

};
#endif

