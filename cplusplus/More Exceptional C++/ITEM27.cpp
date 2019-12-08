ITEM27:(非)纯虚函数
	//什么是纯虚函数？给出一个例子
	// Example 27-1
	//
	class AbstractClass
	{
	  // declare a pure virtual function:
	  // this class is now abstract
	  virtual void f(int) = 0;
	};
	
	class StillAbstract : public AbstractClass
	{
	  // does not override f(int),
	  // so this class is still abstract
	};
	
	class Concrete : public StillAbstract
	{
	public:
	  // finally overrides f(int),
	  // so this class is concrete
	  void f(int) { /*...*/ }
	};
	
	AbstractClass a;    // error, abstract class
	StillAbstract b;    // error, abstract class
	Concrete      c;    // ok, concrete class
	
 //在声明一个纯虚函数后，你为什么还会为他提供定义(函数体)？尽可能给出这样做的理由和场合
 
 //纯虚析构函数
   /*
    * 所有的基类的析构函数要么应该是虚拟共有成员，要么应该是非虚拟保护成员。
    * 简单的说，这是因为，首先，要记住，你应该总是避免从实体类派生，因而假设接力不是实体类
    * 那么，他就不会出于实例化自身的目的去提供一个一个共有析构函数，这样就只剩下
    * 两个选择
    * (1) 要么，你需要通过基类指针进行多态删除的功能，这种情况下析构函数必须是虚拟共有成员
    * (2)要么，你不需要这一功能，这种情况下，析构函数应该是非虚拟公有成员--之所以是保护成员，为了防止滥用
    * 如果一个类应该是抽象类，但是他没有其他任何纯虚函数，而有一个共有析构函数，那么将这个析构函数生命为纯虚函数
    */
    // Example 27-2(a)
		//
		// file b.h
		//
		class B
		{
		public: /*...other stuff...*/
		  virtual ~B() = 0; // pure virtual destructor
		};
		//当然，任何派生类的析构函数必须隐式的调用基类的析构函数，所以析构函数还是得定义，即使为空
		// Example 27-2(a), continued
		//
		// file b.cpp
		//
		
		B::~B() { /* possibly empty */ }
		
		//如果不提供这个定义，你还是可以让其他类从B派生，但那些派生类不能实例化，从而没有什么特别用处
		
		//设计准则:基类的析构函数要么是虚拟共有成员要么是非虚拟保护成员

   //2.明确的使用缺省行为
   //如果派生类没有改写某个普通的虚函数，他就会默认的继承基类中的行为
   //如果想提供一个默认行为但是又不想让派生类这么无声无息的继承，你可以声明一个
   //并且依然提供缺省行为实现，这样派生类的设计者如果想要使用它，就必须主动对他进行调用
   // Example 27-2(b)
		//
		class B
		{
		protected:
		  virtual bool f() = 0;
		};
		
		bool B::f()
		{
		  return true;   // this is a good default, but
		}
		                 // shouldn't be used blindly
		
		class D : public B
		{
		  bool f()
		  {
		    return B::f(); // if D wants the default
		  }
		                   // behaviour, it has to say so
		};
		//3.提供部分行为
		//有时候我们需要想派生类提供部分行为，同时这个派生类还必须
		//保持完整，这是一种很有价值的应用，其设计思想是:在派生类中
		//将基类实现做为派生类实现的一部分来执行
		
		// Example 27-2(c)
		//
		class B
		{
		  // ...
		protected virtual bool f() = 0;
		};
		
		bool B::f()
		{
		  // do something general-purpose
		}
		
		class D : public B
		{
		  bool f()
		  {
		    // first, use the base class's implementation
		    B::f();
		
		    // ... now do more work ...
		  }
		};
		
		
				
