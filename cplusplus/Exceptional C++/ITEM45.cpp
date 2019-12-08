ITEM45:BOOL
	//对bool类型的几个实现方式，但是都是由缺陷的
	
	//1.typedef
	// Option 1: typedef 
	//
	typedef int bool;
	const bool true  = 1;
	const bool false = 0;
	//方法不错，但是不能重载
	// file f.h 
	void f( int  ); // ok
	void f( bool ); // ok, redeclares the same function
	// file f.cpp
	void f( int  ) { /*...*/ }  // ok
	void f( bool ) { /*...*/ }  // error, redefinition
	//2.#define
  // Option 2: #define 
	//这种方法有害，与上面有相同的问题，而且破坏了 #define
	#define bool  int
	#define true  1
	#define false 0
	//3.enum
	// Option 3: enum 
	//
	enum bool { false, true };
	//允许重载，但是条件表达式不允许自动类型转换
	bool b; 
	b = ( i == j );//不能正常工作
 //4.class
 	class bool 
	{
	public:
	  bool();
	  bool( int );            // to enable conversions from
	  bool& operator=( int ); //  conditional expressions
	  //operator int();   // questionable!
	  //operator void*(); // questionable!
	private:
	  unsigned char b_;
	};
	const bool true ( 1 );
	const bool false( 0 );
	//除了转换操作有问题外就可以工作，这些问题原因
	//1.由于自动类型转换，重载解决了bool的冲突
	//2.如果使用自动类型转换，bool类型的数据会影响到函数重载，就像其他任何一个类型的隐式构造函数和自动类型转换一样
	//3.不能转换与int或void*相似的类型，bool在条件中不能做测试
	//4.如果不使用自动转换类型，那么bool类型将无法在使用在条件判断中
	bool b; 
	/*...*/
	if( b ) // error without an automatic conversion to
	{       // something like int or void*
	    /*...*/
	}


	
