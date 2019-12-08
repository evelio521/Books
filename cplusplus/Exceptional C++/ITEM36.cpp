ITEM36:内存管理(二)
	//下面的程序有一些类使用他们自己的内存管理方案，找出错误
	
			class B 
		{
			//为什么B的delete还有第二个参数？
	    //为什么D的delete没有第二个参数？
	    //这两种都可以正常的回收，因个人喜好不同
	    //但是没有提供相应的new 和new[]是非常危险的
		public:
		  virtual ~B();
		  void operator delete  ( void*, size_t ) throw();
		  void operator delete[]( void*, size_t ) throw();
		  void f( void*, size_t ) throw();
		};
		class D : public B
		{
		public:
		  void operator delete  ( void* ) throw();
		  void operator delete[]( void* ) throw();
		};
		
    void f()
    {
    	//下面哥哥语句，到底哪一个delete被调用了？为什么？
    	//调用时的参数是什么
    	D* pd1 = new D; 
			delete pd1;//D::operator delete(void*).
			B* pb1 = new D;
			delete pb1;//D::operator delete(void*). 
			D* pd2 = new D[10];
			delete[] pd2;//D::operator delete[](void*).
			B* pb2 = new D[10];
			delete[] pb2;//不可预料的，C++语言要求，传递给delete的指针的静态类型必须和动态类型一样。


    }
    //下面两个赋值语句合法吗
    B b;
    typedef void (B::*PMF)(void*, size_t); 
		PMF p1 = &B::f;
		PMF p2 = &B::operator delete;
		//第一个赋值语句合法
		//第二个不合法，因为 void operator delete( void*, size_t ) throw() 
		//并不是B的成员函数。因为new和delete是静态的
		
		//小技巧
		//new和delete总是静态的，即使不显示的声明为static，总是把他们
		//声明为static是个好的习惯，可以帮助理解程序
		class X 
		{
		public:
		  void* operator new( size_t s, int )
		                   throw( bad_alloc )
		  //会产生内存泄漏，因为没有响应的placement delete存在。
		  {
		    return ::operator new( s );
		  }
		};
		class SharedMemory
		{
		public:
		  static void* Allocate( size_t s )
		  {
		    return OsSpecificSharedMemAllocation( s );
		  }
		    static void  Deallocate( void* p, int i = 0 )
		    {
		      OsSpecificSharedMemDeallocation( p, i );
		    }
		  };
		  
		  class Y
		  {
		  public:
		    void* operator new( size_t s,
		                        SharedMemory& m ) throw( bad_alloc )
		    {
		      return m.Allocate( s );
		    }
		    //若没有operator delete也会造成内存泄漏，若在构造函数抛出异常就不会释放内存
		   /* void  operator delete( void* p,
		                           SharedMemory& m,
		                           int i ) throw()
		    {
		       m.Deallocate( p, i );
		    }*/
		  };
		  //如下面的构造函数抛出异常
		  SharedMemory shared; 
			...
			new (shared) Y; // if Y::Y() throws, memory is leaked
			
			
			void operator delete( void* p, 
                        SharedMemory& m,
                        int i ) throw()
			  {
			  	//这里的delete毫无用处，因为他不会被调用
			    m.Deallocate( p, i );
			  }
			};
			void operator delete( void* p, 
                      std::nothrow_t& ) throw()
			{
				//这是一个严重的错误，因为他将要删除哪些缺省::operator new分配出来的内存
				//而不是shareMemory::Allocate()分配出来的内存。
			  SharedMemory::Deallocate( p );
			}
			
			void operator delete( void* p, 
                      std::nothrow_t& ) throw()
			{//这也是一样，只是更为微妙。这里delete只会在new(nothrow) T失败的时候才会被调用
			 //因为T的构造函数会带来一个异常来终止，并企图回收那些不是被SharedMemory::Allocate（）分配的内存
			 SharedMemory::Deallocate( p );
			}


			
			      
