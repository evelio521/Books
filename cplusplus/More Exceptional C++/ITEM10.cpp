ITEM10:模版特殊化与重载
	//1.模版特殊化的例子
	
	  //显示特殊化
	  
	  //通用模版
	  template<typename T> void sort( Array<T>& v ) { /*...*/ };
	  //对char* 特殊模版
	  template<> void sort<char*>( Array<char*>& );
		//编译器会选出合适的模版
		Array<int>   ai;
		Array<char*> apc;
		
		sort( ai );       // calls sort<int>
		sort( apc );      // calls specialized sort<char*>
	
  //2.什么是部分特殊化？给出一个例子
  
	//The first template is the primary class template:

	template<typename T1, typename T2, int I>
	class A             { };             // #1
	
	//We can specialize this for the case when T2 is a T1*:
	
	template<typename T, int I>
	class A<T, T*, I>   { };             // #2
	
	//Or for the case when T1 is any pointer:
	
	template<typename T1, typename T2, int I>
	class A<T1*, T2, I> { };             // #3
	
	//Or for the case when T1 is int and T2 is any pointer and I is 5:
	
	template<typename T>
	class A<int, T*, 5> { };             // #4
	
	//Or for the case when T2 is any pointer:
	
	template<typename T1, typename T2, int I>
	class A<T1, T2*, I> { };             // #5

	//编译器会选择合适的模版
		A<int, int, 1>   a1;  // uses #1

		A<int, int*, 1>  a2;  // uses #2, T is int,
		                      //          I is 1
		
		A<int, char*, 5> a3;  // uses #4, T is char
		
		A<int, char*, 1> a4;  // uses #5, T1 is int,
		                      //          T2 is char,
		                      //          I is 1
		
		A<int*, int*, 2> a5;  // ambiguous:
		                      // matches #3 and #5
		//3.C++允许对函数模版进行重载
		template<typename T1, typename T2>
		void g( T1, T2 );                       // 1
		template<typename T> void g( T );       // 2
		template<typename T> void g( T, T );    // 3
		template<typename T> void g( T* );      // 4
		template<typename T> void g( T*, T );   // 5
		template<typename T> void g( T, T* );   // 6
		template<typename T> void g( int, T* ); // 7
		template<> void g<int>( int );          // 8
		void g( int, double );                  // 9
		void g( int );                          // 10
		
		//将问题简化
		
		//接受2个参数
		template<typename T1, typename T2>
		void g( T1, T2 );                       // 1
		template<typename T> void g( T, T );    // 3
		template<typename T> void g( T*, T );   // 5
		template<typename T> void g( T, T* );   // 6
		template<typename T> void g( int, T* ); // 7
		void g( int, double );                  // 9
		//接受一个参数
		template<typename T> void g( T );       // 2
		template<typename T> void g( T* );      // 4
		template<> void g<int>( int );          // 8
		void g( int );                          // 10
		
		//一下每条语句调用上面的哪个函数
		int             i;
		double          d;
		float           f;
		complex<double> c;
		
		g( i );         // a----------10
		g<int>( i );    // b----------8
		g( i, i );      // c----------3
		g( c );         // d----------2
		g( i, f );      // e----------9
		g( i, d );      // f----------9
		g( c, &c );     // g----------6
		g( i, &d );     // h----------7
		g( &d, d );     // i----------5
		g( &d );        // j----------4
		g( d, &i );     // k----------1
		g( &i, &i );    // l----------3
		
		
