ITEM29:使用auto_ptr
	//1.使用auto_ptr的常见错误，下面代码存在什么问题？
	template<typename T>
	void f( size_t n ) {
	  auto_ptr<T> p1( new T );
	  auto_ptr<T> p2( new T[n] );
	
	  // ... more processing ...
	}
	//p2的问题在于 auot_ptr只是用来包含单个对象，所以对于自己拥有的指针。auto_ptr总是
	//会调用delete而不是delete[] ，所以使用普通的delete，p1会被正确的清除，p2则不会。
	
	//如果使用错误的delete，产生的实际后果取决于你的编译器。最好结果是内存泄漏，更常见
	//的是内存被破坏，然后程序崩溃。下面的代码你可以体验一下效果！！！！
	#include <iostream>
	#include <memory>
	#include <string>
	using namespace std;
	int c = 0;
	
	class X {
	public:
	  X() : s( "1234567890" ) { ++c; }
	 ~X()                     { --c; }
	    string s;
	};
	
	template<typename T>
	void f( size_t n )
	{
	  {
	    auto_ptr<T> p1( new T );
	    auto_ptr<T> p2( new T[n] );
	  }
	  cout << c << " ";   // report # of X objects
	}                     // that currently exist
	
	int main()
	{
	  while( true )
	  {
	    f<X>(100);
	  }
	}
	
	//零长度的数组是完全合法的 new T[0] 与new T[n]有一样的行为特征
	//auto_ptr不能拥有零长度数组
	
	
	//2.如何解决这一问题？尽可能提供更多的方案，包括运用Adapter模式，替换
	//有问题结构，替换auto_ptr等
	
	//方案(1) 打造自己的auto_ptr
	//       方案(1)(a)  通过从auto_ptr派生
	          /*
	           * 优点几乎没有
	           * 缺点举不胜举
	           */
	//       方案(1)(b)  通过复制auto_ptr的代码
	          /*
	           * 去除程序库中的auot_ptr代码。将其中的delete改为delete[]
	           * 有点: 易于实现。没有空间或时间上的显著开销
	           * 缺点: 难以维护
	           */
	           
	           
	//方案2:运用Adapter模式
	  
	    //以下是设计思路 我们不这么写
	    auto_ptr<T> p2( new T[n] );
			//而是这么写
			auto_ptr< ArrDelAdapter<T> >
              p2( new ArrDelAdapter<T>(new T[n]) );
      //其中，ArrDelAdapter具有一个参数为T指针的构造函数，
      //在析构函数中，调用delete[]
      template<typename T>
			class ArrDelAdapter {
			public:
			  ArrDelAdapter( T* p ) : p_(p) { }
			 ~ArrDelAdapter() { delete[] p_; }
			  // operators like "->" "T*" and other helpers
			private:
			  T* p_;
			};
			//既然只有一个ArrDelAdapter<T>对象，~auto_ptr()中的单个对象形式的delete就不会有问题
			//因为~ArrDelAdapter<T>对数组正确调用delete[]
			
			//有点:易于实现
			//缺点:可读性性差，难以使用，带来空间上的开销

      //方案3: 用手工编写的异常处理逻辑取代auto_ptr
			//我们手工的为p2数组剥掉auto_ptr这层外衣，手工编写自己的异常处理逻辑
			//我们不这么写
			auto_ptr<T> p2( new T[n] );
			//
			// ... more processing ...
			//
			//我们这么写
			T* p2( new T[n] );
			try {
			  //
			  // ... more processing
			  //
			}
			delete[] p2;
			//有点:易于使用，没有空间上的开销
			//缺点:难以实现，缺乏健壮性，可读性差
			
			//方案4:用vector数组替代
			//我们不这么写
			auto_ptr<T> p2( new T[n] );
			//我们这么写
			vector<T> p2( n );
			//有点: 易于实现，可读性好，程序健壮性提高，没有空间和时间上的开销
			//缺点: 语法改变，使用性改变
			
			//设计准则:尽量使用vector，不要使用内建的指针





			

	
	
