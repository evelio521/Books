ITEM26:多继承与连体双婴问题
	//请看下面的两个类
	class BaseA
	{
	  virtual int ReadBuf( const char* );
	  // ...
	};
	
	class BaseB
	{
	  virtual int ReadBuf( const char* );
	  // ...
	};
	//BaseA和BaseB有一个共同点---他们显然都想被用作基类，但是除此之外他们
	//毫不相干，他们的ReadBuf()函数用来做不同的事情，而且这两个类还来自不同程序供应商
	
	//示范如何写一个Derived类，这个类从BaseA和BaseB共有继承，而且还要对两个ReadBuf()
	//进行改写，让他们做不同的事情
	
	//下面的写法不正确
	// Example 26-1: Attempt #1, doesn't work
		//
		class Derived : public BaseA, public BaseB
		{
		  // ...
		
		  int ReadBuf( const char* );
		      // overrides both BaseA::ReadBuf()
		      // and BaseB::ReadBuf()
		};
		// Example 26-1(a): Counterexample,
		// why attempt #1 doesn't work
		//
		Derived d;
		BaseA*  pba = d;
		BaseB*  pbb = d;
		
		pba->ReadBuf( "sample buffer" );
		    // calls Derived::ReadBuf
		
		pbb->ReadBuf( "sample buffer" );
		    // calls Derived::ReadBuf

		//正确的写法
		// Example 26-2: Attempt #2, correct
		//
		class BaseA2 : public BaseA
		{
		public:
		  virtual int BaseAReadBuf( const char* p ) = 0;
		private:
		  int ReadBuf( const char* p )    // override inherited
		  {
		    return BaseAReadBuf( p );     // to call new func
		  }
		};
		
		class BaseB2 : public BaseB
		{
		public:
		  virtual int BaseBReadBuf( const char* p ) = 0;
		private:
		  int ReadBuf( const char* p )   // override inherited
		  {
		    return BaseBReadBuf( p );    // to call new func
		  }
		};
		
		class Derived : public BaseA2, public BaseB2
		{
		  /* ... */
		
		public: // or "private:", depending whether other
		        // code should be able to call these directly
		
		  int BaseAReadBuf( const char* );
		      // overrides BaseA::ReadBuf indirectly
		      // via BaseA2::BaseAReadBuf
		
		  int BaseBReadBuf( const char* );
		      // overrides BaseB::ReadBuf indirectly
		      // via BaseB2::BaseBReadBuf
		};
		//现在一切正常了
		// Example 26-2(a): Why attempt #2 works
		//
		Derived d;
		BaseA*  pba = d;
		BaseB*  pbb = d;
		
		pba->ReadBuf( "sample buffer" );
		    // calls Derived::BaseAReadBuf
		
		pbb->ReadBuf( "sample buffer" );
		    // calls Derived::BaseBReadBuf
		
		
				
			
			
