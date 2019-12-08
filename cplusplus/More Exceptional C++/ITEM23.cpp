ITEM23:异常安全与类的设计之二:继承
	//1.Is-Implemented-In-Terms-Of 的含义是什么？
	
	//如果T在他的实现中以某种形式使用了另一种类型U，就称T Is-Implemented-In-Terms-Of U。
	//以某种形式使用这一措辞当然留有很大余地，他表示的范围很广，如T可以是U的一个适配器
	//代理或包装类;或者T仅仅只是在他的实现细节中偶尔用到U。
	
	//T IIITO U通常意味着:要么，T有一个U 例如:
	// Example 23-1(a): "T IIITO U" using Has-A
	//
	class T
	{
	  // ...
	private:
	  U* u_;  // or by value or by reference
	};
	//要么T非共有派生于U，例如
	// Example 23-1(b): "T IIITO U" using derivation
	//
	class T : private U
	{
	  // ...
	};
		
		
	//2.在C++中，Is-Implemented-In-Terms-Of可以通过非公有继承或包容/委托来表达。
	//具体来说，在写一个类T的时候，如果它要用类U来实现。两个主要的选择是:
	//让T从U私有继承，或者让T包含一个U成员对象。

	//继承容易被使用过度，一条有效的工程设计原则:将耦合性降至最低，如果一种关系可以用多种有效方式表达
	//请使用关系最弱的那一个，既然继承近似乎是C++中可以表达的最强烈的关系，仅次于友元，那么，只有在
	//没有更弱的关系时，我们才使用它。
	
	//最小耦合性原则无疑会直接影响到代码的健壮性，编译时间，以及其他可见后果。
	//有趣的是，为了实现IIITO ,在继承和委托之间所做的选择还会有异常安全性上的牵连
	//
	
	
	//对异常安全的影响
	//如果想用Has-A来表达IIITO关系，我们应该怎样写T::operator=()。
	// Example 23-2(a): "T IIITO U" using Has-A
	//
	class T
	{
	  // ...
	  private:
	    U* u_;
	};
	//近似强烈安全性的T::operator=()
	T& T::operator=( const T& other )
	{
	   U* temp = new U( *other.u_ );   // do all the work
	                                   //  off to the side
	
	   delete u_;      // then "commit" the work using
	   u_ = temp;      //  nonthrowing operations only
	   return *this;
	}


  //一旦U和T之间的关系涉及到任何方式的继承，问题会有什么变化
  // Example 23-2(b): "T IIITO U" using derivation
	//
	class T : private U
	{
	  // ...
	};
	T& T::operator=( const T& other )
	{
	  U::operator=( other );  // ???如果可以在开始修改目标的情况下抛出异常
	  //我们无法写出具有强烈安全性的T::operator=(),除非U通过其他某个函数提供了合适功能
	  return *this;
	}
	//换句话说，如果T为他的成员函数T::operator=()提供异常安全保证的能力必然
	//依赖与U的安全和保证。在表达T和U之间的关系时，上述代码使用了最大可能的紧密关系
	//从而导致最大可能的高耦合性。


  //总结:
  //松散的耦合性促进程序的正确性(包括异常安全性)，紧密的耦合性降低程序的最大可能
  //正确性(包括异常安全)
