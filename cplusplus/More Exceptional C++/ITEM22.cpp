ITEM22:异常安全与类的设计之一:拷贝赋值
	//异常安全的三个级别
	//1.基本保证
	//2.强烈保证
	//3.不抛出异常的保证
	
	//标准的auto_ptr的一个重要特性是，auto_ptr操作绝不会抛出异常
	
	//具有强烈常安全性的拷贝赋值的规范是什么？
	
	//一个是提供一个不抛出异常的swap()
	void T::Swap( T& other ) /* throw() */
	{
	  // ...swap the guts of *this and other...
	}
  //接着，运用创建一个临时对象然后交换的手法实现operator=()
  T& T::operator=( const T& other )
	{
	  T temp( other ); // do all the work off to the side
	  Swap( temp );    // then "commit" the work using
	  return *this;    //  nonthrowing operations only
	}
	
	//3.请看下面这个类
	// Example 22-1: The Cargill Widget Example
	//
	class Widget
	{
	public:
	  Widget& operator=( const Widget& ); // ???
	  // ...
	private:
	  T1 t1_;
	  T2 t2_;
	};
	//假设T1，T2的某一个操作会抛出异常。如果不改变类的结构，有可能写出具有异常安全的
	//Widget::operator=(const Widget&)吗？
	
	//如果不改变结构，我们无法写出具有异常安全的Widget::operator=(const Widget&)
	
	//4.说明示范一种简单的转换技术:这种转换可以应用于任何一个类
	//并可以很容易的使得那个类的拷贝赋值具有强烈的异常安全训过，其他场合，我们看到过这种技术？
	
	//尽管不改变Widget的结构我们无法写出具有异常安全的Widget::operator=()
	//但是通过下面转换技术，我们可以实现一个具有近乎强烈异常安全的赋值。即
	//通过指针而不是值来拥有成员对象，最好使用Pimpl转换手法，将一切隐藏在
	//单个指针之后
	
	// Example 22-2: The general solution to
	// Cargill's Widget Example
	//
	class Widget
	{
	public:
	  Widget();  //用新的WidgetImpl初始化 pimpl_ 
	
	  ~Widget(); //这个析构函数必须提供，因为隐式生成的版本会
	       //  导致使用上的问题
	       //  (see Items 30 and 31)
	  Widget& operator=( const Widget& );
	
	  // ...
	
	  private:
	    class WidgetImpl;     
	    auto_ptr<WidgetImpl> pimpl_;
	    // ... 提供可以正常工作的拷贝
	    //     构造函数或者进制它 ...
	};
	
	// 然后一般是一个单独的实现文件中
	// implementation file:
	//
	class Widget::WidgetImpl
	{
	public:
	  // ...
	  T1 t1_;
	  T2 t2_;
	};
  //注意，如果使用auto_ptr成员你必须
   //(1) 要么，你必须将WidgetImpl的定义提供给Widget，要么，如果你很想隐藏
       //WidgetImpl，你就必须为Widget写出自己的析构函数，即使这个析构函数很简单  
   //(2)对于Widget，还应该提供自己的拷贝构造函数和赋值函数，一般来说你不希望类的成员
       //具有"拥有权转移"语义。如果你有另外一个智能指针，你可以考虑代替auto_ptr,但是上述原则依然重要
   
   //现在我们可以很容易实现一个不抛出异常的swap，那么我们很容器写出一个近似满足强烈异常安全保证的拷贝赋值函数
   void Widget::Swap( Widget& other ) /* throw() */
	{
	  auto_ptr<WidgetImpl> temp( pimpl_ );
	  pimpl_ = other.pimpl_;
	  other.pimpl_ = temp;
	}
	Widget& Widget::operator=( const Widget& other )
	{
	  Widget temp( other ); // do all the work off to the side
	  Swap( temp );    // then "commit" the work using
	  return *this;    //  nonthrowing operations only
	}









  //得到的结论
  //结论1: 异常安全影响类的设计
  //结论2: 总能让你的代码具有强烈的异常安全性
  //结论3: 明智的使用指针(1)指针是你的敌人，它带来的种种问题是auto_ptr要消除的问题
                      //(2)指针是你的朋友，使用指针不会带来异常
  
