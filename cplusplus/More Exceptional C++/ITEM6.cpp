ITEM6:容器，指针和不是容器的容器
	//1. 下面代码
	vector<char> v;
	// ... populate v ...
	
	char* p = &v[0];
	
	// ... do something with *p ...
	
	//我们总是说能使用迭代器的地方尽量不使用指针，然而有时候使用指针的地方
	//不一定能使用迭代器
	
	//使用迭代器的缺陷
	1.能使用指针的地方不一定能使用迭代器
	2.如果迭代器是一个对象而不是一个普通指针，使迭代器招致空间和性能的开销
	
	//假设有一个map<Name,PhoneNumber> 他在程序启动时载入 此后只用于查询，
	//也就是给出一个名字我们很任意查询出电弧号码。我们要进行反向查询怎们办？
	//一个简单的方法就是在构造一个数据结构 map<PhoneNumber*,Name*,Deref>
	//这样避免了双倍的开销，因为第二个拥有的只是指针
	
	//但是如果用迭代器是无法办到的
	
	//2.下面的代码合法吗？安全吗？ 好吗？
	
	vector<char> c;
	//....填充v....
	char* p=&v[0];
	//....使用*p.....
	
	
	//这段代码完全合法
	//这段代码是安全的 只要我们知道指针什么时候失效
	//这段代码是有意义的，拥有一个指向容器内部的指针是绝对有意义的
	
	
	//对代码的改进
	vector<char> c;
	//....填充v....
	vector<char>::iterator i=c.begin();
	//....使用*i.....
	
	//我们总是说能使用迭代器的地方尽量不使用指针，然而有时候使用指针的地方
	//不一定能使用迭代器
	
	//使用迭代器的缺陷
	1.能使用指针的地方不一定能使用迭代器
	2.如果迭代器是一个对象而不是一个普通指针，使迭代器招致空间和性能的开销
	
	//假设有一个map<Name,PhoneNumber> 他在程序启动时载入 此后只用于查询，
	//也就是给出一个名字我们很任意查询出电弧号码。我们要进行反向查询怎们办？
	//一个简单的方法就是在构造一个数据结构 map<PhoneNumber*,Name*,Deref>
	//这样避免了双倍的开销，因为第二个拥有的只是指针
	
	
	//3. 下面代码合法吗
	template<typename T>
	void f( T& t )
	{
	  typename T::value_type* p1 = &t[0];
	  typename T::value_type* p2 = &*t.begin();
	  // ... do something with *p1 and *p2 ...
	}
	
	//合法
	
	1.To make the expression &t[0] valid, T::operator[]() 
		must exist and must return something that understands
		operator&(), which in turn must return a valid T::value_type*
	 (or something that can be meaningfully converted to a valid 
	 T::value_type*).

   In particular, this is true of containers that meet the standard‘s
    container and sequence requirements and implement the optional 
    operator[](), because that operator must return a reference to 
    the contained object. By definition, you can then take the 
    contained object’s address.

   2.To make the expression &*t.begin() valid, T::begin() must exist, 
   	and it must return something that understands operator*(), 
   	which in turn must return something that understands operator&,
   	which in turn must return a valid T::value_type* (or something 
   	that can be meaningfully converted to a valid T::value_type*).

    //现在的问题是:对标准库中的支持operator[]()的任何容器来说，上面的
    //代码都是合法的，如果去掉&t[0]那行语句，对标准库的每个容器也是成立的
    //唯独std::vector<bool>除外
    
	 //下面的代码除bool外都成立
		// Example 6-3: Also works for every T except bool
		//
		template<typename T>
		void g( vector<T>& v )
		{
		  T* p = &v.front();
		  // ... do something with *p ...
		}
		//vector<bool>被特化，所以他不是一个容器，不符合标准库的条件
		
		
		//谨慎多早的优化
		1.不要太早优化
		2.除非确实必要，否则不要使用优化
		3.即使那样，除非知道想要优化什么，哪里需要优化，否则不要优化
		
		//vector<bool>是特化版本 具体见c++ STL或MSDN