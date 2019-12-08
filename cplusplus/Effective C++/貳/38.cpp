//构造一个template 希望构造出一组classes用来表示不重复对象组成的sets
#include<list>
#include<iostream>
#include<string>
#include<algorithm>//find
using namespace std;
template <class T> class Set{
	
	public:
		bool member(const T& item)const;
		void insert(const T& item);
		void remove(const T& item);
		void display();
		size_t size() const; 
	private:
		list<T> rep;//原来表述Set数据
};

template<typename T>
bool Set<T>::member(const T& item)const
	{
		return find(rep.begin(),rep.end(),item)!=rep.end();
	}
	
template<typename T>
void  Set<T>::insert(const T& item)
	{
		if(!member(item)) rep.push_back(item);
	}
template<typename T>
void  Set<T>::remove(const T& item)
	{
		typename list<T>::iterator it=find(rep.begin(),rep.end(),item);
		if(it!=rep.end()) rep.erase(it);
	}

template<typename T>
size_t  Set<T>::size() const
	{
		return rep.size();
	}
template<typename T>
void  Set<T>::display() 
{
		for(list<T>::iterator iter=rep.begin();iter!=rep.end();iter++)
			cout<<*iter<<endl;
}
int main(){
	
	Set<int> s1;
	s1.insert(5);
	s1.insert(6);
	s1.insert(7);
	s1.insert(8);
	s1.insert(9);
	s1.insert(0);
	s1.display();
	

	
}