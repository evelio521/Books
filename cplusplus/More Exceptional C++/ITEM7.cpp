ITEM7:使用vevtor和deque
	//vector用作数组:一个启发式的例子
	
	//c风格的数组
	// Example 7-1(a): A function that operates
		// on a C-style array
		//
		int FindCustomer(
		  const char* szName,     // name to search for
		  Customer*   pCustomers, // pointer to beginning
		                          //  of a Customer array
		  size_t      nLength )   //  with nLength entries
		{
		  // performs a (possibly optimized) search and
		  // returns the index at which the specified
		  // name was found
		}
		
		//To use the above function, we might create,
		// populate, and pass in an array as follows:
		
		// Example 7-1(b): Using an array
		//
		Customer c[100];
		  //-- populate the contents of c somehow —-
		int i = FindCustomer( "Fred Jones", &c[0], 100 );

   //vector的使用
   vector<Customer> c;
		//-- populate the contents of c somehow —-
		int i = FindCustomer( "Fred Jones", &c[0], c.size() );
    
    //尽量使用vector 不要使用数组
    
    
    //vector是那种应该在默认情况下使用的序列...当大多数插入删除操作发生
    //在序列的开头和结尾时，应该使用deque
    
    //缩小vector的空间
    
    vector<C> c(10000);
    //c.size()=10000 c.capacity()>=10000
    c.erase(c.begin()+10,c.end());
    ////c.size()=10
    c.reserve(10);
    //没有改变vector容器的大小
    
    
    //将vector容器缩小的方法
    
    //
    vector<C> c(10000);
    //c.capacity()>=10000
    c.erase(c.begin()+10,c.end());
    //删除前10个元素外的所有元素
    vector<C>(c).swap(c);
    //缩小至合适的大小
    //现在c.capacity==c.size() 或略大于他
    
    //通常vector或deque会保留而外的空间，以备将来增长的需要，防止增加元素频繁的分配
    //有可能完全清除一个vector或deque吗？
    
    //可以
    // Example 7-3: The right way to clear out a vector.
		//
		vector<Customer> c( 10000 );
		// ...now c.capacity() >= 10000...
		
		// the following line makes
		// c be truly empty
		vector<Customer>().swap( c );
		
		// ...now c.capacity() == 0, unless the
		// implementation happens to enforce a
		// minimum size even for empty vectors

