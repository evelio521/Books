ITEM23:classes之间的关系
	//资料库应用软件往往需要对某个已知的表格内的每一笔记录或是一群圈起来的记录做某些动作
	//，他们常常会执行一些唯读动作，将整个表格走一遍，放进快取装置中，以便获取需要处理的记录
	//然后进入实际动作做些处理。
	
	//程式员不希望一再重复这种常见的逻辑，于是试着提供一个泛型抽象类别提供一个可复用的框架
	//想法是抽象类别将重复动作封装起来，收集需要操作各个操作行，其次在执行必要动作。
	//衍生类别负责提供特定的动作
	
	
	//--------------------------------------------------- 
	// File gta.h
	//---------------------------------------------------
	class GenericTableAlgorithm
	{
	public:
	  GenericTableAlgorithm( const string& table );
	  virtual ~GenericTableAlgorithm();
	
	  // Process() returns true if and only if successful.
	  // It does all the work: a) physically reads
	  // the table's records, calling Filter() on each
	  // to determine whether it should be included
	  // in the rows to be processed; and b) when the
	  // list of rows to operate upon is complete, calls
	  // ProcessRow() for each such row.
	  //
	  bool Process();
	
	private:
	  // Filter() returns true if and only if the row should be
	  // included in the ones to be processed. The
	  // default action is to return true (to include
	  // every row).
	  //
	  virtual bool Filter( const Record& );
	
	  // ProcessRow() is called once per record that
	  // was included for processing. This is where
	  // the concrete class does its specialized work.
	  // (Note: This means every row to be processed
	  // will be read twice, but assume that that is
	  // necessary and not an efficiency consideration.)
	  //
	  virtual bool ProcessRow( const PrimaryKey& ) =0;
	
	  struct GenericTableAlgorithmImpl* pimpl_; // MYOB
	};
	
	//For example, the client code to derive a concrete worker 
	//class and use it in a mainline looks something like this:
	
	class MyAlgorithm : public GenericTableAlgorithm 
	{
	  // ... override Filter() and ProcessRow() to
	  //     implement a specific operation ...
	};
	int main()
	{
	  MyAlgorithm a( "Customer" );
	  a.Process();
	}
	
	//这是个好的设计 使用了哪个设计模式？为什么他在此处有用？
	
	  //这是Template Method pattern。他之所以有用，因为我们只需要遵循相同的步骤，就可以将某个常见解法一般化。
	  //只有细节部分不同，此部分可有衍生版本提供。
	  
	  //设计准则：尽量避免使用public虚拟函数式，最好以template method pattern取代之
	  //设计准则：了解什么是design pattern 并运用之。
	  
	 //2.再不改变基础设计的基础上，试评论此一设计的执行方式，你有什么不同的做法？pimpl_member的目的？
	 
	   //这个设计以bool做为传回码，显然没有其他方法记录失败。试需求而定，这或许是好的，
	   //但是有些东西需要注意。程序中pimpl_member非常巧妙的将实作细节隐藏于一个不透明指标之后
	   //pimpl_指向的结构将内含private member functions和members variable
	   //使得他们的任何改变不至于使得client端有必要重新设计。
	   
	//3. 对这个做法的改善
	
	//设计准则：尽量形成内聚。总是尽力让每一段代码---每一个模组，每一个类别，每一个函数式--由单一而明确的任务
	
	  //--------------------------------------------------- 
		// File gta.h
		//---------------------------------------------------
		// Responsibility #1: Providing a public interface
		// that encapsulates common functionality as a
		// template method. This has nothing to do with
		// inheritance relationships, and can be nicely
		// isolated to stand on its own in a better-focused
		// class. The target audience is external users of
		// GenericTableAlgorithm.
		//
		class GTAClient;
		
		class GenericTableAlgorithm
		{
		public:
		  // Constructor now takes a concrete implementation
		  // object.
		  //
		  GenericTableAlgorithm( const string& table,
		                         GTAClient&    worker );
		
		  // Since we've separated away the inheritance
		  // relationships, the destructor doesn't need to be
		  // virtual.
		  //
		  ~GenericTableAlgorithm();
		
		  bool Process(); // unchanged
		
		private:
		  struct GenericTableAlgorithmImpl* pimpl_; // MYOB
		};
		//---------------------------------------------------
		// File gtaclient.h
		//---------------------------------------------------
		// Responsibility #2: Providing an abstract interface
		// for extensibility. This is an implementation
		// detail of GenericTableAlgorithm that has nothing
		// to do with its external clients, and can be nicely
		// separated out into a better-focused abstract
		// protocol class. The target audience is writers of
		// concrete "implementation detail" classes which
		// work with (and extend) GenericTableAlgorithm.
		//
		class GTAClient
		{
		public:
		  virtual ~GTAClient() =0;
		  virtual bool Filter( const Record& );
		  virtual bool ProcessRow( const PrimaryKey& ) =0;
		};
		
		//---------------------------------------------------
		// File gtaclient.cpp
		//---------------------------------------------------
		bool GTAClient::Filter( const Record& )
		{
		  return true;
		}
		
		
		class MyWorker : public GTAClient 
		{
		  // ... override Filter() and ProcessRow() to
		  //     implement a specific operation ...
		};
		
		int main()
		{
		  GenericTableAlgorithm a( "Customer", MyWorker() );
		  a.Process();
		}
		//注意以下重要的三点
		1.如果 GenericTableAlgorithm的公共公开界面改变了，在原来的设计中，所有具象的worker classes必须重新编译
		  因为他们衍生自 GenericTableAlgorithm。在新版本中， GenericTableAlgorithm公开界面的任何改变都被巧妙的隔离
		  ，一点也不会影响具象的worker classes
		2.如果 GenericTableAlgorithm的可扩充协定改变了，原来的版本中， GenericTableAlgorithm的所有外部client都必须重新编译
		  ，因为再次class定义区可见到衍生界面。但是在最新的版本中， GenericTableAlgorithm的可扩充协议界面的改变被隐藏了，任何
		  变化不会影响外部使用者。
		3.任何具象的worker classes如今可以在其他任何演算法中被使用--只要该演算法能够使用Filter()和ProcessRow()介面来进行--而不是
		  被 GenericTableAlgorithm使用而已。
		  
		  
		  //可以将GenericTableAlgorithm改写为 一个泛式，不必是个classes
		  bool GenericTableAlgorithm( 
		  				const string& table,
		  				GTAClient&    method)
		{
		  // ... original contents of Process() go here ...
		}
		
		int main()
		{
		  GenericTableAlgorithm( "Customer", MyWorker() );
		}
		
		//下面的模版并不好，请拒绝因为个人利益而写出一些险招秘技
		template<typename GTACworker> 
		bool GenericTableAlgorithm( const string& table )
		{
		  // ... original contents of Process() go here ...
		}
		
		int main()
		{
		  GenericTableAlgorithm<MyWorker>( "Customer" );
		}


		
		
				
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
