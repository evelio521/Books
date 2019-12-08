ITEM41:对象的生存周期
	//评述下面的惯用法
	T& T::operator=( const T& other ) 
	{
	  if( this != &other )
	  {
	    this->~T();
	    new (this) T(other);
	  }
	  return *this;
	}
	//1.代码试图达到什么样的合法目的？
	   //这个惯用法是经常被推荐使用的，是C++草案的例子，但是这个例子是有害无益的
	   //这个惯用法以拷贝构造来实现拷贝赋值，该方法保证拷贝构造与拷贝赋值是相同的操作
	   //防止不必要的重复。如果虚拟基类有数据成员这个方法还是挺有用的，其实虚拟基类不该
	   //有数据成员。既然是虚拟基类，说明该类是用于继承而设计的，这意味着我们不能用这种用法
	   
	//上面的代码包含一个可以修正的缺陷和以及若干个无法修正的缺陷
	  //#1 它会切割对象
	  	//如果T是一个带有虚拟析构函数的基类，那么this->~T();就会出现问题这一句就执行了错误的
	  	//操作。如果是对一个派生类对象执行这个调用操作，这一句的执行将会销毁派生类对象并用一个T
	  	//对象代替。而这种结果几乎肯定破坏性的影响后续任何试图使用这个对象的代码。
	  	//看下面的代码
	  	Derived& 
			Derived::operator=( const Derived& other )
			{
			  Base::operator=( other );
			  // ...now assign Derived members here...
			  return *this;
			}
			//这样我们得到
			class U : T { /* ... */ }; 
			U& U::operator=( const U& other )
			{
			  T::operator=( other );
			  // ...now assign U members here...
			  //    ...oops, but we're not a U any more!
			  return *this;  // likewise oops
			}
		  //对T::operator=()的调用一声不响的对其后的代码(包括U成员的赋值操作和返回语句)
		  //产生了破坏性的影响。为了改正这个问题，可以改为this->T::~T();做为替代。但是这仍然是不安全的。
		//#2他不是异常安全的
		  //new调用拷贝构造可抛出异常
		//#3他使赋值操作变得低效
		//#4改变了正常对象的声明周期
		//#5他可以对派生类对象产生破坏性的影响
		//#6依赖于不可靠的指针比较操作
		
	//2.加入修正了所有的缺陷，这种惯用法安全吗？对你的回答作出解释，如果不安全，如何达到目的
	//我们以拷贝赋值来实现拷贝构造
	T::T(const T& other){
		
		/*T:: */ operator=(other);
				
	}
	T& T::operator=(const T& other){
		
		//真正的工作从这里开始
		//大概可以在异常安全的状态下完成。但现在
		//其可以抛出异常，却不像原来那样产生什么不良影响
		return *this;
	}
	//为了代码美观 你可以这样做,和上面的没有区别
	T::T(const T& other){
		do_copy(other);
	}
	
	T& T::operator=(const T& other){
		do_copy(other);
		return *this;
	}
	T& T::do_copy(const T& other){
		//真正的工作从这里开始
		//大概可以在异常安全的状态下完成。但现在
		//其可以抛出异常，却不像原来那样产生什么不良影响
	}
