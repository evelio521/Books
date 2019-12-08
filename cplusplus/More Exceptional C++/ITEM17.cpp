ITEM17:构造函数失败之一:对象的生命期
	//看下面这个类
	// Example 17-1
		//
		class C : private A
		{
		  B b_;
		};
		//在C的构造函数中，如何捕捉从基类镀锡i昂或成员对象构造函数
		//中抛出的异常
		
		
		//这是function try block的用武之地
		// Example 17-1(a): Constructor function try block
		//
		C::C()
		try
		  : A ( /*...*/ )   // optional initialization list
		  , b_( /*...*/ )
		{
		}
		catch( ... )
		{
		  // We get here if either A::A() or B::B() throws.
		
		  // If A::A() succeeds and then B::B() throws, the
		  // language guarantees that A::~A() will be called
		  // to destroy the already-created A base subobject
		  // before control reaches this catch block.
		}

	//下面代码
	// Example 17-2
	//
	{
	  Parrot p;
	}
  //这个对象的生命周期何时开始？
  //当他的构造函数成功执行完毕并正常返回之时，也就是说，当控制抵达构造函数整体末尾之时
  //或是完成更早一个returne语句时
  
  //何时结束？
  //当他的析构函数开始执行之时，也就是说抵达析构函数开始处
  
  //在对象的声明周期外，对象处于什么状态？
  //在生命周期开始之前和结束之后，对象的状态完全一样，没有对象存在
  
  //最后，如果他的构造函数抛出一个异常，那将意味着什么？
  //意味着构造失败，对象没有存在过，他的生命周期没有开始过

  //对于C++的构造函数，总结如下:只会是二者之一
  
  (A) 构造函数正常返回，对象真实存在
  (B) 构造函数抛出异常后退出，对象从未存在。
