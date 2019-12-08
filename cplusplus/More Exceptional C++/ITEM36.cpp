ITEM36:初始化
	//1.直接初始化和拷贝初始化有何不同？
	
	//直接初始化
	U u;
	T t1(u); // calls T::T( U& ) or similar
	//拷贝初始化
	T t2 = t1;  // same type: calls T::T( T& ) or similar
	T t3 = u;   // different type: calls T::T( T(u) )
            //  or T::T( u.operator T() ) or similar

  //设计准则:变量初始化尽量采用T t(u),不要采用T t=u;
  
  //2.下列例子中哪些使用直接初始化，哪些使用拷贝初始化？
  class T : public S
	{
	public:
	  T() : S(1),             // 基类初始化
	        x(2) {}           // 成员初始化
	  X x;
	};
	
	T f( T t )                // 传递函数参数
	{
	  return t;               // 返回值
	}
	
	S s;
	T t;
	S& r = t;
	
	reinterpret_cast<S&>(t);  // 执行 reinterpret_cast
	static_cast<S>(t);        // 执行 static_cast
	dynamic_cast<T&>(r);// 执行 dynamic_cast
	const_cast<const T&>(t);  // 执行 const_cast
	
	try
	{
	  throw T();              // 抛出异常
	}
	catch( T t )              // 处理异常
	{
	}
	
	f( T(s) );                // functional-notation type conversion
	S a[3] = { 1, 2, 3 };     // brace-enclosed initializers
	S* p = new S(4);          // new expression
	
	
	//上面问题的答案
	class T : public S
	{
	public:
	  T() : S(1),              // base initialization
	        x(2) {}            // member initialization
	  X x;
	};
	
	//基类和成员初始化采用直接初始化
	
	T f( T t )                // passing a function argument
	{
	  return t;               // returning a value
	}
	
	//值得传递和返回都是采用拷贝初始化
	
	S s;
	T t;
	S& r = t;
	
	reinterpret_cast<S&>(t);  // performing a reinterpret_cast
	dynamic_cast<T&>(r);      // performing a dynamic_cast
	const_cast<const T&>(t);  // performing a const_cast
	
	//圈套: 这些地方完全没有涉及到新对象初始化，只是创建了引用。
	
	static_cast<S>(t);        // performing a static_cast
	
	A static_cast uses direct initialization.
	
	try
	{
	  throw T();              // throwing an exception
	}
	catch( T t )              // handling an exception
	{
	}
	
	//异常对象的抛出和捕获使用拷贝初始化
	f( T(s) );              //函数形式的类型转换，直接初始化

	
	
	S a[3] = { 1, 2, 3 };   // 大括号的初始化语句，拷贝初始化
	
	Brace-enclosed initializers use copy initialization.
	
	S* p = new S(4);        // new表达式 直接初始化
	
	
	
			
			
				