规则1:
	/*仔细区别pointers和reference*/
	/*
	 * 没有所谓的null reference 一个reference必须代表一个对象
	 * pointer可以设为null
	 * 
	 */
	 
	 //下面的代码是有害的 可能导致不可预期的行为
	  char *pc=0;
	  char& rc=*pc;
	  
	 //c++要求 reference必须有初值
	 string& rs;//错误
	 string s("xyzxyz");
	 string& rs=s;//ok
	 
	 //pointer可以没有初值
	 string* ps;
	 
	 //reference比pointer更富效率 使用前不需要测试有效性
	 void printDouble(const double& rd)
	   {
	   	cout<<rd;
	  	}
	  void printDouble(const double* rd)
	   {
	   	if(rd)
	   	{
	   	cout<<*rd;
	  	}
	  }
	  
	  //pointer可以被重新赋值，而reference却总是指向他或初值的对象
	  
	  string s1("nacy");
	  string s2("clancy");
	  
	  string& rs=s1;
	  string *ps=&s1;
	  rs=s2;
	  
	  ps=&s2;
	  
	  /*
	   *还有一种情况，就是当你重载某个操作符时，你应该使用引用。
	   *最普通的例子是操作符[]。这个操作符典型的用法是返回一个目标对象，其能被赋值。
	   */
		vector<int> v(10); // 建立整形向量（vector），大小为10;
											// 向量是一个在标准C库中的一个模板(见条款M35)
		v[5] = 10;     // 这个被赋值的目标对象就是操作符[]返回的值
		
		//如果操作符[]返回一个指针，那么后一个语句就得这样写：
		
		*v[5] = 10;
	  
	 