ITEM21:未管理指针存在问题之二:使用auto_ptr
	//1.下面代码异常安全还存在吗？较上个主题的代码改进在哪？
	// Example 21-1
	//
	// In some header file:
	void f( auto_ptr<T1>, auto_ptr<T2> );
	
	// In some implementation file:
	f( auto_ptr<T1>( new T1 ), auto_ptr<T2>( new T2 ) );
	//多了auto_ptr之后的，编译器的执行步骤如下(一)
	//(1)为T1分配内存
  //(2)构造T1
  //(3)为T2分配内存
  //(4)构造T2
  //(5)构造auto_ptr<T1>
  //(6)构造auto_ptr<T2>
  //(7)调用f()
  //上面的代码在第三步或第四步任何一步出现问题，上个主题的异常仍然发生
	//多了auto_ptr之后的，编译器的执行步骤如下(二)
	//(1)为T1分配内存
  //(2)为T2分配内存
  //(3)构造T1
  //(4)构造T2
  //(5)构造auto_ptr<T1>
  //(6)构造auto_ptr<T2>
  //(7)调用f()
  //上面的代码在第三步或第四步任何一步出现问题，上个主题的异常仍然发生
  
	//2.演示如何写一个auto_ptr_new工具，来解决(1)中问题，并可以如下调用
	// Example 21-2
		//
		
		// In some header file:
		void f( auto_ptr<T1>, auto_ptr<T2> );
		
		// In some implementation file:
		f( auto_ptr_new<T1>(), auto_ptr_new<T2>() );
		
		//最简单的方案，提供下面一个模版
		// Example 21-2(a): Partial solution
		//
		template<typename T>
		auto_ptr<T> auto_ptr_new()
		{//只对有缺省构造函数有效
		  return auto_ptr<T>( new T );
		}
		//这解决了一场安全问题，他产生的代码不会有资源泄漏问题，因为我没问你只有两个函数，而我们知道，
		//函数不会交叉运行
		
		//请看下面的运算次序,无论哪步出现异常，不会有内存泄漏
		//(1) 调用auto_ptr<T1>()
		//(2)	调用auto_ptr<T1>()
		
		//解决没有缺省函数的问题，时期具有通用性
		// Example 21-2(b): Improved solution
		//
		template<typename T>
		auto_ptr<T> auto_ptr_new()
		{
		  return auto_ptr<T>( new T );
		}
		
		template<typename T, typename Arg1>
		auto_ptr<T> auto_ptr_new( const Arg1& arg1 )
		{
		  return auto_ptr<T>( new T( arg1 ) );
		}
		
		template<typename T, typename Arg1, typename Arg2>
		auto_ptr<T> auto_ptr_new( const Arg1& arg1,
		                          const Arg2& arg2 )
		{
		  return auto_ptr<T>( new T( arg1, arg2 ) );
		}
		
    //一个更好的解决方案，将两个临时的auot_ptr对象分别放进各自的命名变量中
    // Example 21-1(b): A simpler solution
		//
		
		// In some header file:
		void f( auto_ptr<T1>, auto_ptr<T2> );
		
		// In some implementation file:
		{
		  auto_ptr<T1> t1( new T1 );
		  auto_ptr<T2> t2( new T2 );
		  f( t1, t2 );
		}
    
    
    //设计原则: 在各自独立的程序语句中执行每一个显示的资源
    //分配(new)，并将通过(new)分配的资源立即交给管理者对象(auto_ptr)
