#include "Composite.h"
#include "Component.h"
#define NULL 0

Composite::Composite()
{
}
Composite::~Composite()
{
}
void Composite::operation()
{
	vector<Component*>::iterator it=comVec.begin();
	for(;it!=comVec.end();++it)
	{
		(*it)->operation();
	}
}
void Composite::Add( Component *com)
{
	comVec.push_back(com);
}
void Composite::Remove( Component *com)
{
	vector<Component*>::iterator it=comVec.begin();

	for(;it!=comVec.end()&&(*it)!=com;++it);

	if(it!=comVec.end())
	comVec.erase(it);
}
Component* Composite::getChild(int index)
{
	return comVec[index];
}