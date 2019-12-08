ITEM39:名字空间。之一:using声明和using指令
	
	//什么是using声明和using命令?如何使用他们?给出例子。另外
	//他们中的哪一个和顺序相关?
	
	//using声明为实际声明在另一个名字空间中的名称创建一个本地同义词。
	//为了重载和名称解析的需要，using声明的运作方式和其他声明一样
	// Example 39-1(a)
	//
	namespace A
	{
	  int f( int );
	  int i;
	}
	
	using A::f;
	
	int f( int );
	
	int main()
	{
	  f( 1 );     // 歧义: A::f() or ::f()?
	
	  int i;
	  using A::i; // 错误, 重复声明 (像是
	              //  写了两次int i一样)
	}
	//只有已经出现过声明的名称，using声明才能将其引入
	// Example 39-1(b)
	//
	namespace A
	{
	  class X {};
	  int Y();
	  int f( double );
	}
	using A::X;
	using A::Y; // 函数 A::Y(), 非类 A::Y
	using A::f; //  A::f(double), 非 A::f(int)
	
	namespace A
	{
	  class Y {};
	  int f( int );
	}
	
	int main()
	{
	  X x;      // 正确, X 是 A::X同义词
	
	  Y y;      // 错误, A::Y 不可见 因为
	            //  using声明出现在他想要
	            //  的实际声明之前
	
	  f( 1 );   // 这里偷偷使用了一个隐式转换，
	            //  他调用的是 A::f(double),而不是
	            //  A::f(int), 因为在using声明之前
	            //  只有头一个 A::f() 声明出现过
	             //  seen at the point of the using-
	             //  declaration for the name A::f
	}
	
	//有了using指令，另一个名字空间中的所有名称都可以使用于using指令所在的空间，
	//和using声明不同，using指令会将声明在using指令之前，之后所有的名称都引入进
	//来。当然他在使用某个名称之前，那个名称必须出现过才行。
	// Example 39-1(c)
	//
	namespace A
	{
	  class X {};
	  int f( double );
	}
	void f()
	{
	  X x;      // OK, X is a synonym for A::X
	  Y y;      // error, no Y has been seen yet
	  f( 1 );   // OK, calls A::f(double) with parameter promotion
	}
	
	using namespace A;
	
	namespace A
	{
	  class Y {};
	  int f( int );
	}
	
	int main()
	{
	  X x;      // OK, X is a synonym for A::X
	  Y y;      // OK, Y is a synonym for A::Y
	  f( 1 );   // OK, calls A::f(int)
	}
	
	
		
