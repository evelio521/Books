ITEM32-34:接口原则
	//*** Example 1 (a) 
	class X { /*...*/ };
	/*...*/
	void f( const X& );
	
	//*** Example 1 (b) 
	class X
	{
	  /*...*/
	public:
	  void f() const;
	};
	
	//接口原则：对于一个类X，包括自由函数，只要同时满足
	//1.提到X，并且
	//２.与X同期提供(如Example 1 (a))
	//那么就是X的逻辑组成部分，因为他们形成了X的接口
	//那么根据定义所有的成员函数都是X的组成部分，因为
	//１.每个成员函数都必须提到X（非static成员函数有隐含的X* cont 或const X* const）
	//2.每个成员函数都和X同期提供
	
	
	//*** Example 1 (c) 
		class X { /*...*/ };
		/*...*/
		ostream& operator<<( ostream&, const X& );
		
		//如果operator<<与X同期提供，那么他就是X的逻辑组成部分
		
		//class的定义:一个class描述了一组数据及操作这些数据的函数
		
		//考虑下面的例子
		/*** Example 2 (a) ***/ 
		struct _iobuf { /*...data goes here...*/ };
		typedef struct _iobuf FILE;
		FILE* fopen ( const char* filename,
		              const char* mode );
		int   fclose( FILE* stream );
		int   fseek ( FILE* stream,
		              long  offset,
		              int   origin );
		long  ftell ( FILE* stream );
		     /* etc. */
		//考虑把下面的例子用class重写
		//*** Example 2 (b) 
		class FILE
		{
		public: 
		  FILE( const char* filename,
		        const char* mode );
		~FILE();
		  int  fseek( long offset, int origin );
		  long ftell();
		       /* etc. */
		private:
		  /*...data goes here...*/
		};
		//当然我们可以将函数实现一部分为成员函数，一部分为非成员函数
		//*** Example 2 (c) 
		class FILE
		{
		public:
		  FILE( const char* filename,
		        const char* mode );
		~FILE();
		  long ftell();
		       /* etc. */
		private:
		  /*...data goes here...*/
		};
		int fseek( FILE* stream,
		           long  offset,
		           int   origin );
		
		//是不是成员函数不是问题的关键，只要他提及FILE并且与FILE同期提供，他
		//就是FILE的组成部分
				
				
	//介绍Koenig Lookup
	//看下面的例子
	//*** Example 3 (a) 
		namespace NS
		{
		  class T { };
		  void f(T);
		}
		NS::T parm;
		int main()
		{
		  f(parm);    // OK: calls NS::f
		}
		//如果你传给函数一个class类型的实参，为了查找这个函数名，编译器被要求不仅要搜索如局部作用域这样的常规空间，还要搜索
		//包含实参类型的命名空间。
		
		//*** Example 3 (b) 
		#include <iostream>
		#include <string>  // this header declares the function
		                   //   std::operator<< for strings
		int main()
		{
		  std::string hello = "Hello, world";
		  std::cout << hello;   // OK: calls std::operator<<
		}
		//如果没有Koenig Lookup 编译器无法找到operator<<.
		//我们必须写std::operator<<(std::cout,hello);或 using namespace std;


		//看下面的例子
		//*** Example 4 (a) 
		namespace NS   // this part is
		{             // typically from
		  class T { }; // some header
		}             // file T.h
		void f( NS::T );
		int main()
		{
		  NS::T parm;
		  f(parm);     // OK: calls global f
		}
		//
		//*** Example 4 (b) 
		namespace NS   // typically from
		{             // some header T.h
		  class T { };
		  void f( T ); // <-- new function
		}
		void f( NS::T );
		int main()
		{
		  NS::T parm;
		  f(parm);     // ambiguous: NS::f
		}             //   or global f?
		//在命名空间增加一个函数破坏了命名空间外的代码

		//*** The Myers Example: "Before" 
		namespace A
		{
		  class X { };
		}
		namespace B
		{
		  void f( A::X );
		  void g( A::X parm )
		  {
		    f(parm);   // OK: calls B::f
		  }
		}//这很好，但是当下面的代码出现时
		//*** The Myers Example: "After" 
		namespace A
		{
		  class X { };
		  void f( X ); // <-- new function
		}
		namespace B
		{
		  void f( A::X );
		  void g( A::X parm )
		  {
		    f(parm);   // ambiguous: A::f or B::f?
		  }
		}
		
		//如果代码提供了一个提及X的函数，而他与X所处的命名空间提供某个函数签名重合时，
		//调用就会有二义性，看下面的代码
		namespace A
		{
		  class X { };
		  ostream& operator<<( ostream&, const X& );
		}
		namespace B
		{
		  ostream& operator<<( ostream&, const A::X& );
		  void g( A::X parm )
		  {
		    cout << parm; // ambiguous: A::operator<< or
		  }              //   B::operator<<?
		}

		
		//解决方法
		//*** NOT the Myers Example 
		namespace A
		{
		  class X { };
		  void f( X );
		}
		class B      // <-- class, not namespace
		{
		  void f( A::X );
		  void g( A::X parm )
		  {
		    f(parm); // OK: B::f, not ambiguous
		  }
		};
		
		
		//看下面两组代码
		//*** Example 5 (a) -- nonvirtual streaming 
		class X
		{
		  /*...ostream is never mentioned here...*/
		};
		ostream& operator<<( ostream& o, const X& x )
		{
		  /* code to output an X to a stream */
		  return o;
		}
		//*** Example 5 (b) -- virtual streaming 
		class X
		{
		  /*...*/
		public:
		  virtual ostream& print( ostream& ) const;
		};
		ostream& X::print( ostream& o ) const
		{
		  /* code to output an X to a stream */
		  return o;
		}
		ostream& operator<<( ostream& o, const X& x )
		{
		  return x.print( o );
		}
		//选择a的好处是，依赖性低。没有虚函数调用开销
		//选择b的好处是，可以正确的打印出派生类
		
		//但是从接口原则来看 我们应该选择b，两种情况都有依赖性
		
		//一些有趣甚至诧异的结果
		//通常，如果A和B都是class，并且f(A,B)是一个自由函数
		//1.如果A与f同期提供，那么f是A的组成部分，并且A将依赖B
		//2.如果B与f同期提供，那么f是B的组成部分，并且B将依赖A
		//3.如果A，B，f都是同期提供的，那么f同时是A和B的组成部分，并且
		    //A与B是循环依赖。如果一个库提供了两个class及同时涉及二者的操作
		    //那么这哥操作必须规定被同时使用。
		
		
		//如果A与B都是class，并且Ａ::ｇ(Ｂ)是A的成员函数
		//１.因为A::g(B)的存在，A总是依赖B
		//２.如果A与B是同期提供的，那么Ａ::ｇ(Ｂ)也是同期提供的。因为Ａ::ｇ(Ｂ)
		  //同时满足提及B和与B同期提供，那么Ａ::ｇ(Ｂ)是B的组成部分，而Ａ::ｇ(Ｂ)使用了一个
		  //A*参数，所以B依赖A，因为A也依赖B，意味着A,B循环依赖。
		
		
		//*** Example 6 (a) 
		//---file a.h---
		namespace N { class B; }// forward decl
		namespace N { class A; }// forward decl
		class N::A { public: void g(B); };
		//---file b.h---
		namespace N { class B { /*...*/ }; }
		//A的用户包含了a.h，于是A和B是同期提供的并且是循环依赖的。B用户包含了b.h
		//于是A和B不是同期提供的

								
								
		//总结
		1.接口原则：对于class X ,所有函数，包括自由函数，只要同时满足
		   a)提及X
		   b)与X同期提供
		   那么他就是X的逻辑组成部分，因为他是X的接口一部分。						
		2.非成员函数和成员函数都是class的逻辑组成部分，只不过成员数比非成员函数有
		  更强的关联关系
		3.	接口原则中，对同期提供最有用的解释是:出现在相同的头文件或命名空间中。				
										
								
