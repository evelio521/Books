ITEM7:ʹ��vevtor��deque
	//vector��������:һ������ʽ������
	
	//c��������
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
		  //-- populate the contents of c somehow ��-
		int i = FindCustomer( "Fred Jones", &c[0], 100 );

   //vector��ʹ��
   vector<Customer> c;
		//-- populate the contents of c somehow ��-
		int i = FindCustomer( "Fred Jones", &c[0], c.size() );
    
    //����ʹ��vector ��Ҫʹ������
    
    
    //vector������Ӧ����Ĭ�������ʹ�õ�����...�����������ɾ����������
    //�����еĿ�ͷ�ͽ�βʱ��Ӧ��ʹ��deque
    
    //��Сvector�Ŀռ�
    
    vector<C> c(10000);
    //c.size()=10000 c.capacity()>=10000
    c.erase(c.begin()+10,c.end());
    ////c.size()=10
    c.reserve(10);
    //û�иı�vector�����Ĵ�С
    
    
    //��vector������С�ķ���
    
    //
    vector<C> c(10000);
    //c.capacity()>=10000
    c.erase(c.begin()+10,c.end());
    //ɾ��ǰ10��Ԫ���������Ԫ��
    vector<C>(c).swap(c);
    //��С�����ʵĴ�С
    //����c.capacity==c.size() ���Դ�����
    
    //ͨ��vector��deque�ᱣ������Ŀռ䣬�Ա�������������Ҫ����ֹ����Ԫ��Ƶ���ķ���
    //�п�����ȫ���һ��vector��deque��
    
    //����
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

