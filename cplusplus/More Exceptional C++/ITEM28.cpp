ITEM28:受控的多态
	//看下面的代码
	class Base
	{
	public:
	  virtual void VirtFunc();
	  // ...
	};
	
	class Derived : public Base
	{
	public:
	  void VirtFunc();
	  // ...
	};
	
	void SomeFunc( const Base& );
	//另外还有两个函数f1()和f2()，我们的目标是，允许f1()在接受Base对象的地方
  //多态使用Derived对象，但防止其他所有函数包括(f2())这样做
  void f1()
	{
	  Derived d;
	  SomeFunc( d ); // works, OK
	}
	void f2()
	{
	  Derived d;
	  SomeFunc( d ); // we want to prevent this
	}
  //演示如何达到这一效果
  
   //我们之所以能够在接受Base对象的地方多态的使用Derived对象，原因在于Derived
   //从Base共有继承。相反如果Derived从Base私有继承，那么几乎没有代码可以多态的将
   //Derived做为Base使用。之所以说几乎，原因在于:如果源码可以访问Derived的私有成员，
   //他还是可以访问Derived的私有基类，因而可以动态的将Derived代替为Base使用，正常来说
   //只有Derived的成员函数具有访问权。然后通过C++的友元特性，我们可以将类似的访问权扩充
   //到其他外部代码中。
   
   
   //想要得到题目中的效果
    class Derived : private Base
		{
		public:
		  void VirtFunc();
		  // ...
		  friend void f1();
		};
		
