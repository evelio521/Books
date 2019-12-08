ITEM40:对象的生存周期
  //评述下面的程序段，#2处代码是安全或合法的吗？
  void f() 
	{
	  T t(1);
	  T& rt = t;
	  //--- #1: do something with t or rt ---
	  t.~T();
	  new (&t) T(2);
	  //--- #2: do something with t or rt ---
	}// t is destroyed again

  //是的 #2 处的代码是安全且合法的，但是
   a)   函数做为一个整体，他是不安全得，而且
   b)   这样是一个坏习惯
   
   //C++标准草案明确规定，允许这种代码出现。现场的析构和重构造不会使rt
   //这个引用失效。(当然，你不能在t.~T()与placement new之间使用t或rt,因为在那段时期
   //不存在任何对象。我们还假设T::operator&()没有被重载，即没有用来做返回对象之
   //地址以外的其他事情)。因此只考虑这段代码，#2是安全的，但是f()做为一个整体
   //可能不是异常安全的
   
   
   //为什么函数是不安全的
   //因为如果在T(2)处有异常，那么由于t是自动变量，所以t在末尾时候仍然调用T::~T()
   //t被销毁了2次，所以不安全的
   
   
   //在实际中，这种技术根本不会被使用，考虑下面的代码
   // Can you spot the subtle problem? 
		//
		void T::DestroyAndReconstruct( int i )
		{
		  this->~T();
		  new (this) T(i);
		}
		//这种技术基本上来说不算是安全的，看下列代码
		class U : public T { /* ... */ }; 
		void f()
		{
		  /*AAA*/ t(1);
		  /*BBB*/& rt = t;
		  //--- #1: do something with t or rt ---
		  t.DestroyAndReconstruct(2);
		  //--- #2: do something with t or rt ---
		}// t is destroyed again
		
		//如果/*AAA*/ 是T ，那么#2处代码仍然可行，即使/*BBB*/不是T(/*BBB*/可能是B的基类)
		//如果/*AAA*/ 是U(而不是T) ，那么无论/*BBB*/是什么，都毫无悬念。
		//总之，最好不要用这种写法。