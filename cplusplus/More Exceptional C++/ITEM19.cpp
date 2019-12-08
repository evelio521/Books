ITEM19:未捕获的异常
	//std::uncaught_exception()完成什么功能
	
	//标准uncaught_exception()函数提供一种方法，让你知道当前是否有一个异常正处于活动状态。
	
	//抛出异常的析构函数带来的问题
	// Example 19-1: The problem
	//
	class X {
	public:
	  ~X() { throw 1; }
	};
	
	void f() {
	  X x;
	  throw 2;
	} // calls X::~X (which throws), then calls terminate()
	//如果一个异常已经处于活动状态，如果此时析构函数抛出异常，程序会终止，这不是件好事
	
	
	//请看下面的代码
	// Example 19-2: The wrong solution
	//
	T::~T()
	{
	  if( !std::uncaught_exception() )
	  {
	    // ... code that could throw ...
	  }
	  else
	  {
	    // ... code that won't throw ...
	  }
	}
	//这个错误的方案为什么不正确？
	//上面的代码不会想你想象的那样做，这是因为即使是在可以安全抛出异常的时候
	//他也会采用那条不抛出异常的执行路径。
	
	// Example 19-2(a): Why the wrong solution is wrong
	//
	U::~U()
	{
	  try
	  {
	    T t;
	    // do work
	  }
	  catch( ... )
	  {
	    // clean up
	  }
	}
	//如果在异常传播期间，由于堆栈展开使得U对象被摧毁，那么T::~T()将不会
	//使用那条会抛出异常的执行路径，即使此时可以安全的抛出异常，因为T::~T()
	//不知道在这种情况下，他已经受到了外部U::~U()代码块的保护
	
	//另一种不同形式的错误方案
	// Example 19-3: Variant, another wrong solution
	//
	Transaction::~Transaction()
	{
	  if( uncaught_exception() )
	  {
	   RollBack();
	  }
	  else
	  {
	   // ...       }
	}
  //同样，在堆栈展开期间，如果一个被调用的析构函数用到了Transaction，这段代码不会做正确的事情。
  // Example 19-3(a): Why the variant wrong solution
	// is still wrong
	//
	U::~U() {
	  try {
	    Transaction t( /*...*/ );
	    // do work
	  } catch( ... ) {
	    // clean up
	  }
	}
  //总之，Example 19-2不会像你想象的那样工作。
  //正确的方案
  // Example 19-4: The right solution
	//
	T::~T() /* throw() */
	{
	  // ... code that won't throw ...
	}
	//另一个正确的方案
	// Example 19-5: Alternative right solution
	//
	T::Close()
	{
	  // ... code that could throw ...
	}
	
	T::~T() /* throw() */
	{
	  try
	  {
	    Close();
	  }
	  catch( ... ) { }
	}
	//设计准则: 决不允许异常从析构函数抛出，写析构函数时，就像他已经有了一个throw()
	          //异常规范一样
	//设计准则: 如果析构函数调用了一个可能会抛出异常的函数，一定要让这个调用包装在try/catch block
	          //中，以防止异常逃出析构函数


  //关于uncaught_exception() ，建议最好不要使用
	


