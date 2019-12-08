/*通过复合塑膜出has-a或"根据某物实现出"*/

class Address {...};
class PhoneNumber {...};
class Person{
	public:
		...
	private:
		string name;
		Address address;
		PhoneNumber voiceNumber;
		PhoneNumber faxNumber;
};

//构造一个template 希望构造出一组classes用来表示不重复对象组成的sets

template <class T> class Set{
	
	public:
		bool member(const T& item)const;
		void insert(const T& item);
		void remove(const T& item);
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
