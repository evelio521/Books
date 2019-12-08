ITEM31:智能指针成员之二:设计valuePtr
	//写一个合适的ValuePtr模版，他可以向下面这样使用
	// Example 31-1
	//
	class X
	{
	  // ...
	private:
	  ValuePtr<Y> y_;
	};
	//我们将考虑三种情况，在所有三种情况中，构造函数的好处还是得保证:消除工作自动运行，X::X()的设计者
	//只需要做少量的工作就可以保证异常安全性，避免构造函数失败造成的内存泄漏，同样在所有的三种情况中，对于析构函数的
	//限制依然存在，要么Y的完整定义必须和X如影随形，要么必须显示的提供X的析构函数，即使函数体空
	
	//分别适应以下三种特定场合
	//(a) 不允许对ValuePtr进行拷贝和赋值，这是在没什么好处
	// Example 31-2(a): Simple case: ValuePtr without
	// copying or assignment.
	//
	template<typename T>
	class ValuePtr
	{
	public:
	  explicit ValuePtr( T* p = 0 ) : p_( p ) { }
	
	  ~ValuePtr() { delete p_; }
	//当然还得有什么途径来访问指针，所以就像auto_ptr那样，提供下面的代码
	T& operator*() const { return *p_; }

  T* operator->() const { return p_; }
	//不需要reset和release两个函数
	void Swap( ValuePtr& other ) { swap( p_, other.p_ ); }
	//将构造函数声明为explicit是一种好的做法，他避免隐式转换，对于这种转换，ValuePtr的用户是绝不需要的
	private:
	  T* p_;
	
	  // no copying
	  ValuePtr( const ValuePtr& );
	  ValuePtr& operator=( const ValuePtr& );
	};
	
	//(b)允许对ValuePtr进行拷贝和赋值，且具有这样的语义，在创建其Y对象的拷贝时
	//使用的是Y的拷贝构造函数
	
	//下面代码能满足你的需要，但是不具有通用性，但为拷贝构造和拷贝赋值提供了定义
	// Example 31-2(b): ValuePtr with copying and
	// assignment, take 1.
	//
	template<typename T>
	class ValuePtr
	{
	public:
	  explicit ValuePtr( T* p = 0 ) : p_( p ) { }
	
	  ~ValuePtr() { delete p_; }
	
	  T& operator*() const { return *p_; }
	
	  T* operator->() const { return p_; }
	
	  void Swap( ValuePtr& other ) { swap( p_, other.p_ ); }
	
	  //--- new code begin ------------------------------
	  ValuePtr( const ValuePtr& other )
	    : p_( other.p_ ? new T( *other.p_ ) : 0 ) { }
	
		
	  ValuePtr& operator=( const ValuePtr& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	  //--- new code end --------------------------------
	
	private:
	  T* p_;
	};
	//我们允许将来可以在不同类型的ValuePtr之间进行拷贝和赋值，这会对上面的代码带来哪些影响？
	//也就是说我们希望 ，如果X可以转换为Y，就可以将ValuePtr<X>拷贝和赋值给
	//ValuePtr<Y>
	//答案是这种影响可以做到最小。我们可以复制出拷贝构造函数和拷贝赋值函数的
	//模版化版本，这只需要在他们前面加上template<typename U>,并且取一个类型为
	//ValuePtr<U>&的参数
	
	// Example 31-2(c): ValuePtr with copying and
	// assignment, take 2.
	//
	template<typename T>
	class ValuePtr
	{
	public:
	  explicit ValuePtr( T* p = 0 ) : p_( p ) { }
	
	  ~ValuePtr() { delete p_; }
	
	  T& operator*() const { return *p_; }
	
	  T* operator->() const { return p_; }
	
	  void Swap( ValuePtr& other ) { swap( p_, other.p_ ); }
	
	  ValuePtr( const ValuePtr& other )
	    : p_( other.p_ ? new T( *other.p_ ) : 0 ) { }
	
	  ValuePtr& operator=( const ValuePtr& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	  //--- new code begin ------------------------------
	  template<typename U>
	  ValuePtr( const ValuePtr<U>& other )
	    : p_( other.p_ ? new T( *other.p_ ) : 0 ) { }
	
	  template<typename U>
	  ValuePtr& operator=( const ValuePtr<U>& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	private:
	  template<typename U> friend class ValuePtr;
	  //--- new code end --------------------------------
	
	T* p_;
	};
	//这里我们还得写出非模版形式的拷贝和赋值函数，这样做是为了禁止自动生成的版本。
	
	//这里还有一个问题，无论模版形式或非模版形式的拷贝和赋值函数，在我们执行切割操作的
	//情况下，源对象other还是可以拥有一个指向派生类型的指针，下面是例子
	class A {};
	class B : public A {};
	class C : public B {};
	
	ValuePtr<A> a1( new B );
	ValuePtr<B> b1( new C );
	
	// 调用拷贝构造函数 切割
	ValuePtr<A> a2( a1 );
	
	// 调用模版化构造函数 切割
	ValuePtr<A> a3( b1 );
	
	// 调用拷贝赋值函数 切割
	a2 = a1;
	
	// 调用模版化赋值函数 切割
	a3 = b1;
	
	//31-2(b)和31-2(c) 都是正确的 看你权衡，避免设计过度
	
	
	//3.允许对ValuePtr进行拷贝和赋值，且具有这样的语义:如果Y提供一个virtual Y::Clone()
	//函数，在创建Y对象的拷贝时，将使用这个函数，否则，如果Y没有提供这样一个函数，将使用
	//Y的拷贝构造函数
	// Example 31-2(d): 允许拷贝赋值的ValuePtr，对 Example 31-2(c)
	// 做了少量的修改
	//
	template<typename T>
	class ValuePtr
	{
	public:
	  explicit ValuePtr( T* p = 0 ) : p_( p ) { }
	
	  ~ValuePtr() { delete p_; }
	  T& operator*() const { return *p_; }
	
	  T* operator->() const { return p_; }
	
	  void Swap( ValuePtr& other ) { swap( p_, other.p_ ); }
	
	  ValuePtr( const ValuePtr& other )
	    : p_( CreateFrom( other.p_ ) ) { } // changed
	
	  ValuePtr& operator=( const ValuePtr& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	  template<typename U>
	  ValuePtr( const ValuePtr<U>& other )
	    : p_( CreateFrom( other.p_ ) ) { } // changed
	
	  template<typename U>
	  ValuePtr& operator=( const ValuePtr<U>& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	private:
	  //--- new code begin ------------------------------
	  template<typename U>
	  T* CreateFrom( const U* p ) const
	  {
	    return p ? new T( *p ) : 0;
	  }
	  //--- new code end --------------------------------
	
	  template<typename U> friend class ValuePtr;
	
	  T* p_;
	};
	
	
	// Example 31-2(e): 允许拷贝和赋值ValuePtr
	// 提供基于traits的完整定制功能
	// 
	//
	//--- new code begin --------------------------------
	template<typename T>
	class VPTraits
	{
	static T* Clone( const T* p ) { return new T( *p ); }
	};
	//--- new code end ----------------------------------
	
	  template<typename T>
	class ValuePtr
	{
	public:
	  explicit ValuePtr( T* p = 0 ) : p_( p ) { }
	
	  ~ValuePtr() { delete p_; }
	
	  T& operator*() const { return *p_; }
	
	  T* operator->() const { return p_; }
	
	  void Swap( ValuePtr& other ) { swap( p_, other.p_ ); }
	
	  ValuePtr( const ValuePtr& other )
	    : p_( CreateFrom( other.p_ ) ) { }
	
	  ValuePtr& operator=( const ValuePtr& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	  template<typename U>
	  ValuePtr( const ValuePtr<U>& other )
	    : p_( CreateFrom( other.p_ ) ) { }
	
	  template<typename U>
	  ValuePtr& operator=( const ValuePtr<U>& other )
	  {
	    ValuePtr temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	private:
	  template<typename U>
	  T* CreateFrom( const U* p ) const
	  {
	  //--- new code begin ----------------------------
	    return p ? VPTraits<U>::Clone( p ) : 0;
	  //--- new code end ------------------------------
	  }
	
	  template<typename U> friend class ValuePtr;
	
	  T* p_;
	};
	
	//应用实例
	// Example 31-3: Sample usage of ValuePtr.
	//
	class X
	{
	public:
	  X() : y_( new Y(/*...*/) ) { }
	
	  ~X() { }
	
	  X( const X& other ) : y_( new Y(*(other.y_) ) ) { }
	
	  void Swap( X& other ) { y_.Swap( other.y_ ); }
	
	  X& operator=( const X& other )
	  {
	    X temp( other );
	    Swap( temp );
	    return *this;
	  }
	
	private:
	  ValuePtr<Y> y_;
	};
	//设计准则:一般情况下尽量提高设计的通用性，但应该避免设计过度
	
				
					
								
						
								  
								
