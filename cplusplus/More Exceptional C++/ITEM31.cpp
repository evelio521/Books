ITEM31:����ָ���Ա֮��:���valuePtr
	//дһ�����ʵ�ValuePtrģ�棬����������������ʹ��
	// Example 31-1
	//
	class X
	{
	  // ...
	private:
	  ValuePtr<Y> y_;
	};
	//���ǽ����������������������������У����캯���ĺô����ǵñ�֤:���������Զ����У�X::X()�������
	//ֻ��Ҫ�������Ĺ����Ϳ��Ա�֤�쳣��ȫ�ԣ����⹹�캯��ʧ����ɵ��ڴ�й©��ͬ�������е���������У���������������
	//������Ȼ���ڣ�ҪôY��������������X��Ӱ���Σ�Ҫô������ʾ���ṩX��������������ʹ�������
	
	//�ֱ���Ӧ���������ض�����
	//(a) �������ValuePtr���п����͸�ֵ��������ûʲô�ô�
	// Example 31-2(a): Simple case: ValuePtr without
	// copying or assignment.
	//
	template<typename T>
	class ValuePtr
	{
	public:
	  explicit ValuePtr( T* p = 0 ) : p_( p ) { }
	
	  ~ValuePtr() { delete p_; }
	//��Ȼ������ʲô;��������ָ�룬���Ծ���auto_ptr�������ṩ����Ĵ���
	T& operator*() const { return *p_; }

  T* operator->() const { return p_; }
	//����Ҫreset��release��������
	void Swap( ValuePtr& other ) { swap( p_, other.p_ ); }
	//�����캯������Ϊexplicit��һ�ֺõ���������������ʽת������������ת����ValuePtr���û��Ǿ�����Ҫ��
	private:
	  T* p_;
	
	  // no copying
	  ValuePtr( const ValuePtr& );
	  ValuePtr& operator=( const ValuePtr& );
	};
	
	//(b)�����ValuePtr���п����͸�ֵ���Ҿ������������壬�ڴ�����Y����Ŀ���ʱ
	//ʹ�õ���Y�Ŀ������캯��
	
	//������������������Ҫ�����ǲ�����ͨ���ԣ���Ϊ��������Ϳ�����ֵ�ṩ�˶���
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
	//���������������ڲ�ͬ���͵�ValuePtr֮����п����͸�ֵ����������Ĵ��������ЩӰ�죿
	//Ҳ����˵����ϣ�� �����X����ת��ΪY���Ϳ��Խ�ValuePtr<X>�����͸�ֵ��
	//ValuePtr<Y>
	//��������Ӱ�����������С�����ǿ��Ը��Ƴ��������캯���Ϳ�����ֵ������
	//ģ�滯�汾����ֻ��Ҫ������ǰ�����template<typename U>,����ȡһ������Ϊ
	//ValuePtr<U>&�Ĳ���
	
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
	//�������ǻ���д����ģ����ʽ�Ŀ����͸�ֵ��������������Ϊ�˽�ֹ�Զ����ɵİ汾��
	
	//���ﻹ��һ�����⣬����ģ����ʽ���ģ����ʽ�Ŀ����͸�ֵ������������ִ���и������
	//����£�Դ����other���ǿ���ӵ��һ��ָ���������͵�ָ�룬����������
	class A {};
	class B : public A {};
	class C : public B {};
	
	ValuePtr<A> a1( new B );
	ValuePtr<B> b1( new C );
	
	// ���ÿ������캯�� �и�
	ValuePtr<A> a2( a1 );
	
	// ����ģ�滯���캯�� �и�
	ValuePtr<A> a3( b1 );
	
	// ���ÿ�����ֵ���� �и�
	a2 = a1;
	
	// ����ģ�滯��ֵ���� �и�
	a3 = b1;
	
	//31-2(b)��31-2(c) ������ȷ�� ����Ȩ�⣬������ƹ���
	
	
	//3.�����ValuePtr���п����͸�ֵ���Ҿ�������������:���Y�ṩһ��virtual Y::Clone()
	//�������ڴ���Y����Ŀ���ʱ����ʹ������������������Yû���ṩ����һ����������ʹ��
	//Y�Ŀ������캯��
	// Example 31-2(d): ��������ֵ��ValuePtr���� Example 31-2(c)
	// �����������޸�
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
	
	
	// Example 31-2(e): �������͸�ֵValuePtr
	// �ṩ����traits���������ƹ���
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
	
	//Ӧ��ʵ��
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
	//���׼��:һ������¾��������Ƶ�ͨ���ԣ���Ӧ�ñ�����ƹ���
	
				
					
								
						
								  
								
