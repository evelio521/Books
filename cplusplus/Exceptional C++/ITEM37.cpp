ITEM37:auto_ptr
	//下面代码哪些是好的？哪些是安全的？哪些是合法的额？哪些是非法的？
	auto_ptr<T> source() 
	{
	  return auto_ptr<T>( new T(1) );
	}
	void sink( auto_ptr<T> pt ) { }
	void f()
	{
	  auto_ptr<T> a( source() );
	  sink( source() );
	  sink( auto_ptr<T>( new T(1) ) );
	  vector< auto_ptr<T> > v;
	  v.push_back( auto_ptr<T>( new T(3) ) );
	  v.push_back( auto_ptr<T>( new T(4) ) );
	  v.push_back( auto_ptr<T>( new T(1) ) );
	  v.push_back( a );
	  v.push_back( auto_ptr<T>( new T(2) ) );
	  sort( v.begin(), v.end() );
	  cout << a->Value();
	}
	class C
	{
	public:    /*...*/
	protected: /*...*/
	private:   /*...*/
	  auto_ptr<CImpl> pimpl_;
	};
	
	//auto_ptr的最初动机是:如果下面的语句遇到异常没有执行delete语句时候也安全
	// Example 1(a): Original code 
	//
	void f()
	{
	  T* pt( new T );
	  /*...more code...*/
	  delete pt;
	}
	//修改后的代码
	// Example 1(b): Safe code, with auto_ptr 
	//
	void f()
	{
	  auto_ptr<T> pt( new T );
	  /*...more code...*/
	} // cool: pt's destructor is called as it goes out
	  // of scope, and the object is deleted automatically
	
	//一些例子
	//Finally, using an auto_ptr is just about as easy as 
	//using a builtin pointer, and to "take back" the resource 
	//and assume manual ownership again, we just call release().
	
	// Example 2: Using an auto_ptr 
	//
	void g()
	{
	  T* pt1 = new T;
	  // right now, we own the allocated object
	  // pass ownership to an auto_ptr
	  auto_ptr<T> pt2( pt1 );
	  // use the auto_ptr the same way
	  // we'd use a simple pointer
	  *pt2 = 12;       // same as "*pt1 = 12;"
	  pt2->SomeFunc(); // same as "pt1->SomeFunc();"
	  // use get() to see the pointer value
	  assert( pt1 == pt2.get() );
	  // use release() to take back ownership
	  T* pt3 = pt2.release();
	  // delete the object ourselves, since now
	  // no auto_ptr owns it any more
	  delete pt3;
	} // pt2 doesn't own any pointer, and so won't
	  // try to delete it... OK, no double delete
	
	//Finally, we can use auto_ptr's reset() function to reset 
	//the auto_ptr to own a different object. If the auto_ptr 
	//already owns an object, though, it first deletes the already-owned 
	//object, so calling reset() is much the same as destroying 
	//the auto_ptr and creating a new one that owns the new object.
	
	// Example 3: Using reset() 
	//
	void h()
	{
	  auto_ptr<T> pt( new T(1) );
	  pt.reset( new T(2) );
	    // deletes the first T that was
	    // allocated with "new T(1)"
	} // finally, pt goes out of scope and
	  // the second T is also deleted
	
  
  // Example 4(b): A safer Pimpl, using auto_ptr 
		//
		// file c.h
		//
		class C
		{
		public:
		  C();
		  ~C();
		  /*...*/
		private:
		  struct CImpl; // forward declaration
		  auto_ptr<CImpl> pimpl_;
		  C& operator = ( const C& );
		  C( const C& );
		};
		// file c.cpp
		//
		struct C::CImpl { /*...*/ };
		C::C() : pimpl_( new CImpl ) { }
		C::~C() {}
		
		// Example 5: Transferring ownership from 
		//            one auto_ptr to another
		//
		void f()
		{
		  auto_ptr<T> pt1( new T );
		  auto_ptr<T> pt2;
		  pt1->DoSomething(); // OK
		  pt2 = pt1;  // now pt2 owns the pointer,
		              // and pt1 does not
		  pt2->DoSomething(); // OK
		} // as we go out of scope, pt2's destructor
		  // deletes the pointer, but pt1's does nothing
		
		
		// Example 6: Never try to do work through 
		//            a non-owning auto_ptr
		//
		void f()
		{
		  auto_ptr<T> pt1( new T );
		  auto_ptr<T> pt2;
		  pt2 = pt1;  // now pt2 owns the pointer, and
		              // pt1 does not
		  pt1->DoSomething();
		              // error: following a null pointer
		}
		
		//下面分析开头部分提出的问题
		
		auto_ptr<T> source() 
		{
		  return auto_ptr<T>( new T(1) );//安全合法的做法
		}
		void sink( auto_ptr<T> pt ) { }//安全合法的做法

		void f()
		{
		  auto_ptr<T> a( source() );//安全合法的做法
		  sink( source() );//安全合法的做法
		  sink( auto_ptr<T>( new T(1) ) );//安全合法的做法
		  vector< auto_ptr<T> > v;//将auto_ptr放入容器是不安全的且不合法，对于auto_ptr拷贝是不对等的
		  v.push_back( auto_ptr<T>( new T(3) ) );
		  v.push_back( auto_ptr<T>( new T(4) ) );
		  v.push_back( auto_ptr<T>( new T(1) ) );
		  v.push_back( a );
		  v.push_back( auto_ptr<T>( new T(2) ) );
		  sort( v.begin(), v.end() );//不安全不合法
		  cout << a->Value();//不安全不合法
		}
			
		class C
		{
		public:    /*...*/
		protected: /*...*/
		private:   /*...*/
		  auto_ptr<CImpl> pimpl_;//安全合法的做法
		};
		
		//精修后的auto_ptr也使得拷贝const auto_ptr非法
		const auto_ptr<T> pt1( new T ); 
    // making pt1 const guarantees that pt1 can
    // never be copied to another auto_ptr, and
    // so is guaranteed to never lose ownership
		auto_ptr<T> pt2( pt1 ); // illegal
		auto_ptr<T> pt3;
		pt3 = pt1;              // illegal
		pt1.release();          // illegal
		pt1.reset( new T );     // illegal
		

								
							
								
								
								
								
								
