/*ͨ��������Ĥ��has-a��"����ĳ��ʵ�ֳ�"*/

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

//����һ��template ϣ�������һ��classes������ʾ���ظ�������ɵ�sets

template <class T> class Set{
	
	public:
		bool member(const T& item)const;
		void insert(const T& item);
		void remove(const T& item);
		size_t size() const; 
	private:
		list<T> rep;//ԭ������Set����
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
