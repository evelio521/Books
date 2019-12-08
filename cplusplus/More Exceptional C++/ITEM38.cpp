ITEM38:typedef
	//1.为什么使用typedef,尽可能的列举使用场合和理由。
	  /*
	   * 1.便于打字，名字越短，打字越容易。
	   * 2.typedef便于代码阅读
	   *   int ( *t(int) )( int* );
     *   用type的简化后为:
     *   typedef int (*Func)( int* );
     *   Func t( int );
     *   这是一个函数声明，此函数的名称为t，输入参数为int,返回值为一个函数
     *   指针，指向一个输入参数为int，返回值为int的函数
	   * 3.便于交流，typedef有利于表达程序的意图
	   *   int x;
		 *	 int y;
		 *			y = x * 3;   // might be okay -- who knows?
		 *		
		 *		比较下面的代码
		 *		
		 *		typedef int Inches;
		 *		typedef int Dollars;
		 *    Inches  x;
		 *		Dollars y;
		 *		y = x * 3;   // hmmm...?
		 * 4.可移植性，对于那些和平台相关的名称或不具有可移植性的名称使用typedef
		 *    #if defined USING_COMPILER_A
		 *       typedef __int32 Int32;
		 *       typedef __int64 Int64;
		 *    #elif defined USING_COMPILER_B
		 *       typedef int       Int32;
		 *       typedef long long Int64;
     *    #endif
     * /


	//2.在用到了标准容器的代码中，使用typedef为什么是个好主意？
	5.灵活性，较之整个代码修改名称，仅在一处修改typedef更容易
    /*
     *   void f( vector<Customer>& custs )
		 *   {
		 *      vector<Customer>::iterator i = custs.begin();
		 *       ...
		 *    }
     *   与下面代码相比较
     *    typedef vector<Customer> Customers;
		 *    typedef Customers::iterator CustIter;
		 *
		 *     ...
		 *
		 *     void f( Customers& custs )
		 *     {
		 *       CustIter i = custs.begin();
		 *       ...
		 *      }
		 */
		