ITEM26-28:编译期的安全性
	//大多数程序员包含的头文件都比实际应用的多，这将严重影响程序建立的时间。
	 #include "a.h" //class A
	 #include "b.h" //class B
	 #include "c.h" //class C
	 #include "d.h" //class D
	 //注意只有A和C有虚拟函数
	 #include<iostream>
	 #include<ostream>
	 #include<sstream>
	 #include<list>
	 #include<string>
	 
	 class X : public A{
	 	
	 	public:
	 		X (const &);
	 		D Function1(int char*);
	 		D Function1(int ,C);
	 		B& Function2(B);
	 		void Function3(std::wostringstream&);
	 		std::ostream& print(std::ostream&)const;
	 	private:
	 		std::string name_;
	 		std::list<C> clist_;
	 		D            d_;
	 		
	};
	std::ostream& print(std::ostream& os,const X& x){
		
		return x.print(os);
	}
	
	class Y : private B{
	public;
		C Function4(A);
	private:
		std::list<std::wostringstream*> alist_;
	};
	
	//我们可以直接去掉的头文件有
	1.iostream 程序虽然用到流，但是没有用到流的实际东西
	2.ostream和sstream,程序的参数和返回类型被前置声明是可以的，只需要iosfwd就可以了
	
	//我们不能去掉的头文件
	1. a.h
	2. b.h
	3. c.h
	4. d.h
	
	//我们可以通过X和Y使用pimp_的方法去掉 d.h  list string  c.h
	
	//主要的事项:即使ostream没有被定义，内联的operator<<也可能仍然保持其内联性，并使用ostream参数。
	//这是因为你只在调用成员函数的时候才真正的需要相应的定义，当你想接收一个对象并将其当成在
	//函数调用式的参数而不做任何其他额外的事情时，你并不需要该函数的定义
	
	//B是Y的private基类，而且B没有虚函数，因此b.h是可以去掉的。有一个主要的原因我们想使用private继承
	//那就是重载虚拟函数。这里应该让Y拥有一个B的成员。要去掉b.h 我们应该让Y的这个类型为B的成员存在于Y中隐藏的pimp_部分
	
	//X和Y两个类之间没有任何关系，因此我们至少可以把X和Y的定义分别放到两个不同的头文件中去
	//为了不影响有的代码的使用，我们还应该把现有的头文件作为一个存根，让其包含x.h 和y.h
	//如此我们至少可以让y.h不用#include "a.h" 因为现在它只把A用作函数的参数类型，不需要A的定义。
	
	 
	 //改进后的代码
	 
	 //x.h
	 #include "a.h"
	 #include<iosfwd>
	 
	 class C;
	 class D;
	 
	 class X : public A{
	 	
	 	public:
	 		X (const &);
	 		D Function1(int char*);
	 		D Function1(int ,C);
	 		B& Function2(B);
	 		void Function3(std::wostringstream&);
	 		std::ostream& print(std::ostream&)const;
	 	private:
	 		class XImpl* pimpl_;
	 		
	};
	inline std::ostream& print(std::ostream& os,const X& x){
		//这里不需要ostream 的定义
		return x.print(os);
	}
	
	//y.h 没有 #include
	class A ;
	class C;
	class Y{
		public:
			C Function4(A);
		private:
			C YImpl* pimp_;
	};
	
	//got007.h 做为存根包含两个#include 又通过x.h附带另外两个#include
	#include "x.h"
	#include "y.h"
	
	//got007.cpp
	
	struct XImpl{//声明的时候用class 这里可以用struct
		
		std::string name_;
		std::list<C> clist_;
		D          d_;
	};
	struct YImpl{//声明的时候用class 这里可以用struct
		
		
		std::list<std::wostringstream*> alist_;
		B          b_;
	};
	//到现在X使用者只需要#include包含a.h 和iosfwd.Y的使用者只需要包含a.h 和iosfwd，即使后来更新代码包含y.h并去掉got007.h,也不用多加#include
	
	
	
	
	
	//英文版的有点不同哦
	//  x.h: original header 
	//
	#include <iostream>
	#include <ostream>
	#include <list>
	// None of A, B, C, D or E are templates.
	// Only A and C have virtual functions.
	#include "a.h"  // class A
	#include "b.h"  // class B
	#include "c.h"  // class C
	#include "d.h"  // class D
	#include "e.h"  // class E
	class X : public A, private B
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  std::list<C> clist_;
	  D            d_;
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	
  //1.Remove iostream.
	//2.Replace ostream with iosfwd.
	//3.Replace e.h with a forward declaration.

	//根据以上改变后代码
	//  x.h: sans gratuitous headers 
	//
	#include <iosfwd>
	#include <list>
	// None of A, B, C or D are templates.
	// Only A and C have virtual functions.
	#include "a.h"  // class A
	#include "b.h"  // class B
	#include "c.h"  // class C
	#include "d.h"  // class D
	class E;
	class X : public A, private B
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  std::list<C> clist_;
	  D            d_;
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	
	//We had to leave a.h and b.h. We couldn't get rid of these because X 
	//inherits from both A and B, and you always have to have full definitions
	// for base classes so that the compiler can determine X's object size, 
	//virtual functions, and other fundamentals. (Can you anticipate how to
	// remove one of these? Think about it: Which one can you remove, and why/how? 
	//The answer will come shortly.)

  //We had to leave list, c.h, and d.h. We couldn't get rid of these right 
  //away because a list<C> and a D appear as private data members of X.
  // Although C appears as neither a base class nor a member, it is being
  // used to instantiate the list member, and most current compilers require
  // that when you instantiate list<C>, you are able to see the definition of C. 
  //(The standard doesn't require a definition here, though, so even if the 
  //compiler you are currently using has this restriction, you can expect the 
  //restriction to go away over time.)


  //使用pimp_之后
  //  x.h: after converting to use a Pimpl 
	//
	#include <iosfwd>
	#include "a.h"  // class A (has virtual functions)
	#include "b.h"  // class B (has no virtual functions)
	class C;
	class E;
	class X : public A, private B
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  struct XImpl;
	  XImpl* pimpl_;
	    // opaque pointer to forward-declared class
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	//  Implementation file x.cpp 
	//
	struct X::XImpl
	{
	  std::list<C> clist_;
	  D            d_;
	};
	
	
	//  x.h: after removing unnecessary inheritance 
	//
	#include <iosfwd>
	#include "a.h"  // class A
	class B;
	class C;
	class E;
	class X : public A
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  struct XImpl;
	  XImpl* pimpl_; // this now quietly includes a B
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	
	
	
	//编码标准
	1.封装和隔离
	2.在声明类的时候，应该避免暴露出私有成员
	3.应该使用一个形如struct XxxxImpl* impl_ 的不透明的指针来存储私有成员(包括状态变量和成员函数)
	  class Map{
	  	private:
	  		struct MapImpl* impl_;
	  };
		
		
