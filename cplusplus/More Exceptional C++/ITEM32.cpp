ITEM32:递归声明
	//1.什么是函数指针？如何使用他？
	
	//函数指针是可以让你动态的指向具有某种原型的函数，例如:
	// Example 32-1
	//
	
	// Create a typedef called FPDoubleInt for a function
	// signature that takes a double and returns an int.
	//
	typedef int (*FPDoubleInt)( double );
	
	// Use it.
	//
	int f( double ) { /* ... */ }
	int g( double ) { /* ... */ }
	int h( double ) { /* ... */ }
	
	FPDoubleInt fp;
	fp = f;
	fp( 1.1 );     // calls f()
	fp = g;
	fp( 2.2 );     // calls g()
	fp = h;
	fp( 3.14 );    // calls h()
	
		//2.假设可以写出这么一个函数，他能返回指向自身的指针。这个函数同样可以返回
		//一个指针，指向任何一个和他具有相同原型的函数，这种功能有什么用？
		
		//为了实现一部状态机，有时候将每一个状态写成一个函数就够了
		//所有状态函数有相同的原型，他们都返回一个指针，指向下一个将要调用得
		//函数
		
		//此处应该画出状态机图
		
		//下面是一个极度简化的代码片段，演示上面的设计思想
		// Example 32-2
		//
		StatePtr Start( const string& input );
		StatePtr S2   ( const string& input );
		StatePtr Stop ( const string& input );
		StatePtr Error( const string& input ); // error state
		StatePtr Start( const string& input )
		{
		  if( input == "a" )
		  {
		    return S2;
		  }
		  else if( input == "be" )
		  {
		    return Stop;
		  }
		  else
		  {
		    return Error;
		  }
		}
		//3.有可能写出一个函数f()，使其返回自身的指针吗？在下面这种很自然的使用下
		//他应该很有用
		// Example 32-3
		//
		// FuncPtr is a typedef for a pointer to a
		// function with the same signature as f()
		//
		FuncPtr p = f();    // executes f()
		(*p)();             // executes f()
		//如果有可能，请演示如何实现，如果不可能，说明为什么？
		
		//是的，有可能实现，但是方法不是那么显而易见
		
		// Example 32-3(a): 天真的尝试 不正确
		//
		typedef FuncPtr (*FuncPtr)(); // error
		
		// Example 32-3(b): 非标准且不具有移植性
		//这并不是一个解决方案，不满足问题要求，而且很危险
		//
		typedef void* (*FuncPtr)();
		
		void* f() { return (void*)f; }  // cast to void*
		FuncPtr p = (FuncPtr)(f());     // cast from void*
		p();
		
		// Example 32-3(c): 符合标准且具有可移植性
		// 但是是有害的
		//
		typedef void (*VoidFuncPtr)();
		typedef VoidFuncPtr (*FuncPtr)();
		
		VoidFuncPtr f() { return (VoidFuncPtr)f; }
		                          // cast to VoidFuncPtr
		FuncPtr p = (FuncPtr)f(); // cast from VoidFuncPtr
		p();
		
		//下面是一个正确可移植性的方案
		// Example 32-3(d): A correct solution
		//
		class FuncPtr_;
		typedef FuncPtr_ (*FuncPtr)();
		class FuncPtr_
		{
		public:
		  FuncPtr_( FuncPtr p ) : p_( p ) { }
		  operator FuncPtr() { return p_; }
		private:
		  FuncPtr p_;
		};
		
		//可以很自然的声明，定义和使用f()
		
		FuncPtr_ f() { return f; } // natural return syntax
		
		int main()
		{
		  FuncPtr p = f();  // natural usage syntax
		  p();
		}
		
		
				
