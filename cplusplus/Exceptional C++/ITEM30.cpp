ITEM30:FAST PIMPL技术
	
	//标准的malloc和new调用开销很大
	// Attempt #1 
	//
	// file y.h
	#include "x.h"
	class Y
	{
	  /*...*/
	  X x_;
	};
	
	// file y.cpp
	Y::Y() {}
	
  //这个class Y的申明需要class X 的申明已经可见。为了避免这个条件
  // Attempt #2 
	//
	// file y.h
	class X;
	class Y
	{
	  /*...*/
	  X* px_;
	};
	
	// file y.cpp
	#include "x.h"
	Y::Y() : px_( new X ) {}
	Y::~Y() { delete px_; px_ = 0; }

	//这很好的隐藏了X的，但是当Y被广泛使用时，动态内存的开销降低了性能。
	//下面是一个看似很好的解决方案，其实是糟糕的
	// Attempt #3 
	//
	// file y.h
	class Y
	{
	  /*...*/
	  static const size_t sizeofx = /*some value*/;
	  char x_[sizeofx];
	};
	
	// file y.cpp
	#include "x.h"
	Y::Y()
	{
	  assert( sizeofx >= sizeof(X) );
	  new (&x_[0]) X;
	}
	Y::~Y()
	{
	  (reinterpret_cast<X*>(&x_[0]))->~X();
	}
	
	
	//FAST PIMPL技术
	// file y.h 
	class Y
	{
	  /*...*/
	  struct YImpl;
	  YImpl* pimpl_;
	};
	
	// file y.cpp
	#include "y.h"
	struct Y::YImpl
	{
	  /*...private stuff here...*/
	  static void* operator new( size_t )   { /*...*/ }
	  static void  operator delete( void* ) { /*...*/ }
	};
	X::X() : pimpl_( new XImpl ) {}
	X::~X() { delete pimpl_; pimpl_ = 0; }
	
	//小心
	//这个虽然好，但也不要乱用FAST PIMPL。你得到了最佳的内存分配速度。但是别忘记代价:
	//维护这些链表通常导致空间效能下降，因为比通常情况造成更多的碎片。
		
	//当你证明确实需要他时，在使用。
			