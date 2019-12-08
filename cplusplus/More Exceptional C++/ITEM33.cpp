ITEM33:模拟嵌套函数
	//1.什么是嵌套类?他有什么用处？
	//嵌套类是一个包含在另一个类中的类，例如
	// Example 33-1: Nested class
	//
	class OuterClass
	{
	/*...public, protected, or private...*/:
	  class NestedClass
	  {
	    // ...
	  };
	
	  // ...
	  };
	//在组织代码和控制访问访问权限和从属关系方面，嵌套类有很多用处。
	//和类中的其它部分一样，嵌套类也遵守访问规则。上面NestedClass
	//被声明为public，那么任何外部代码可以称之为OuterClass::NestedClass，
	//通常嵌套类是私有实现细节。因而会被声明为private。如果NestedClass
	//为private，那么只有OuterClass的成员和友元可以使用NestedClass
	
	//什么是局部类？有什么用处？
	//局部类是一个定义在一个函数范围内的类--任何函数，无论是成员函数还是自由函数，例如
	// Example 33-2: Local class
	//
	int f()
	{
	  class LocalClass
	  {
	    // ...
	  };
	
	  // ...
	};
	//只有f()的内部代码知道LocalClass并能够使用它，当LocalClass
	//是f()的内部实现细节，因而永远不应该公开给其他代码的时候，他就显示出价值
	
	//在使用非局部类大多数的地方都可以使用局部类，但是局部类或未命名的类不能作为
	//模版参数使用。
	
	template<class T>
	class X { /* ... */ };
	void f()
	{
	  struct S { /* ... */ };
	  X<S> x3;  // error: local type used as
	              //  template-argument
	    X<S*> x4; // error: pointer to local type
	              //  used as template-argument
	  }
	
		 //3.C++不支持嵌套函数，也就说我们不能写出下面的代码
		// Example 33-3
		//
		int f( int i )
		{
		  int j = i*2;
		
		  int g( int k )  // not valid C++
		  {
		    return j+k;
		  }
		
		  j += 4;
		
		  return g( 3 );
		}
		//要解决上面的难题，大多数人会从下面入手
		// Example 33-3(a): Na?ve "local function object"
		// approach (doesn't work)
		//
		int f( int i )
		{
		  int j = i*2;
		
		  class g_
		  {
		  public:
		    int operator()( int k )
		    {
		      return j+k;   // error: j isn't accessible
		    }
		  } g;
		
		  j += 4;
		
		  return g( 3 );
		}
		//上面代码的缺陷是，局部对象不能访问外围函数的变量
		
		//进一步尝试，将函数中所有变量的指针或引用提供给这个局部类
		// Example 33-3(b): Na?ve "local function object plus
		// references to variables" approach (complex,
		// fragile)
		//
		int f( int i )
		{
		  int j = i*2;
		
		  class g_
		  {
		  public:
		    g_( int& j ) : j_( j ) { }
		
		    int operator()( int k )
		    {
		      return j_+k;  // access j via a reference
		    }
		
		  private:
		    int& j_;
		  } g( j );
		
		  j += 4;
		
		  return g( 3 );
		}
		//这个方案可行，但只是勉强可行，这个方案脆弱，难以扩充
		//准确的说，我们只能将它看作一种非常手段。例如要增加一个变量
		//我们将要做四个改动
		//(1) 增加这个变量
		//(2) 为g_增加一个相应的私有引用成员
		//(3) 为g_增加一个相应的构造函数参数
		//(4) 为g::g_()增加一个相应的构造函数参数
		
		//下面是一份略有改善的方案
		// Example 33-3(c): A better solution
		//
		int f( int i )
		{
		  class g_
		  {
		  public:
		    int j;
		
		    int operator()( int k )
		    {
		      return j+k;
		    }
		  } g;
		
		  g.j = i*2;
		  g.j += 4;
		  return g( 3 );
		}
		
		//快接近正确的方案
		// Example 33-3(d): Nearly there!
		//
		int f( int i )
		{
		  // Define a local class that wraps all
		  // local data and functions.
		  //
		  class Local_
		  {
		  public:
		    int j;
		
		    // All local functions go here:
		    //
		    int g( int k )
		    {
		      return j+k;
		    }
		    void x() { /* ... */ }
		    void y() { /* ... */ }
		    void z() { /* ... */ }
		  } local;
		
		  local.j = i*2;
		  local.j += 4;
		
		  local.x();
		  local.y();
		  local.z();
		
		  return local.g( 3 );
		}
		
		//一个完整的极具有扩充性的方案
		// Example 33-3(e): A complete and nicely
		// extensible solution
		//
		class f
		{
		  int  retval; // f's "return value"
		  int  j;
		  int  g( int k ) { return j + k; };
		  void x() { /* ... */ }
		  void y() { /* ... */ }
		  void z() { /* ... */ }
		
		public:
		  f( int i )   // original function, now a constructor
		    : j( i*2 )
		  {
		    j += 4;
		    x();
		    y();
		    z();
		    retval = g( 3 );
		  }
		  operator int() const // returning the result
		  {
		    return retval;
		  }
		};
		
		//注意这个方案可以很容易扩充为成员函数。假设f()不是自由函数
		//而是成员函数，我们想在f()中写一个嵌套函数g(),如下
		// Example 33-4: 这不是合法的C++代码，但他演示
		//               了我们的需要，一个局部函数存在
		//                于一个成员函数之中
		//
		class C
		{
		  int data_;
		
		public:
		  int f( int i )
		  {
		    // a hypothetical nested function
		    int g( int i ) { return data_ + i; }
		
		    return g( data_ + i*2 );
		  }
		};
		
		//要表达这样的关系，我们可以将f()变成一个类。向33-3(e)那样
		//只不过，33-3(e)的那个类实在全局空间，而现在他是一个嵌套类
		//并需要通过辅助函数来访问
		// Example 33-4(a):完整且极具扩充性
		//                 的解决方案，现在用于成员函数
		//
		class C
		{
		  int data_;
		  friend class C_f;
		public:
		  int f( int i );
		};
		
		class C_f
		{
		  C*  self;
		  int retval;
		  int g( int i ) { return self->data_ + i; }
		
		public:
		  C_f( C* c, int i ) : self( c )
		  {
		    retval = g( self->data_ + i*2 );
		  }
		
		  operator int() const { return retval; }
		};
		
		int C::f( int i ) { return C_f( this, i ); }
		
		//设计原则: 力求清晰，避免复杂设计，避免招致困惑
				
					
										
												
														
																
