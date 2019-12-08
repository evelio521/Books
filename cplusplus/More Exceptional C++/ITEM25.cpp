ITEM25:模拟多继承
	//看下面的例子
	class A
	{
	public:
	  virtual ~A();
	  string Name();
	private:
	  virtual string DoName();
	};
	
	class B1 : virtual public A
	{
	  string DoName();
	};
	
	class B2 : virtual public A
	{
	  string DoName();
	};
	
	A::~A() {}
	string A::Name(){   { return DoName(); }
	string A::DoName()  { return "A"; }
	string B1::DoName() { return "B1"; }
	string B2::DoName() {    return "B2"; }
	
	class D : public B1, public B2
	{
	   string DoName() { return "D";  }
	};
	
	//不运用MI，写一个与上面等价的类D，即，在不使用多继承的情况下，演示你能找到
	//拐弯抹角解决问题的方法，让D得到相同的效果和可用性，同时又要避免对调用者代码的句法
	//多做修改，能做到吗？
	
	//首先考虑下面程序的情况，然后开始设计
	void f1( A&  x ) { cout << "f1:" << x.Name() << endl; }
	void f2( B1& x ) { cout << "f2:" << x.Name() << endl; }
	void f3( B2& x ) { cout << "f3:" << x.Name() << endl; }
	
	void g1( A   x ) { cout << "g1:" << x.Name() << endl; }
	void g2( B1  x ) { cout << "g2:" << x.Name() << endl; }
	void g3( B2  x ) { cout << "g3:" << x.Name() << endl; }
	
	int main()
	{
	  D   d;
	  B1* pb1 = &d;   // D* -> B* conversion
	  B2* pb2 = &d;
	  B1& rb1 = d;    // D& -> B& conversion
	  B2& rb2 = d;
	
	  f1( d );        // polymorphism
	  f2( d );
	  f3( d );
	
	  g1( d );        // slicing
	  g2( d );
	  g3( d );
	
	                  // dynamic_cast/RTTI
	  cout << ( (dynamic_cast<D*>(pb1) != 0) ? "ok " : "bad " );
	  cout << ( (dynamic_cast<D*>(pb2) != 0) ? "ok " : "bad " );
	
	  try
	  {
	    dynamic_cast<D&>(rb1);
	    cout << "ok ";
	  }
	  catch(...)
	  {
	    cout << "bad ";
	  }
	
	  try
	  {
	    dynamic_cast<D&>(rb2);
	    cout << "ok ";
	  }
	  catch(...)
	  {
	    cout << "bad ";
	  }
	}

  //下面的方案和我们的要求非常接近
  class D : public B1
	{
	public:
	  class D2 : public B2
	  {
	  public:
	    void   Set ( D* d ) { d_ = d; }
	  private:
	    string DoName();
	    D* d_;
	  } d2_;
	
	  D()                 { d2_.Set( this ); }
	
	  D( const D& other ) : B1( other ), d2_( other.d2_ )
	                      { d2_.Set( this ); }
	
	  D& operator=( const D& other )
	                      {
	                        B1::operator=( other );
	                        d2_ = other.d2_;
	                        return *this;
	                      }
	
	  operator B2&()      { return d2_; }
	
	  B2& AsB2()          { return d2_; }
	
	private:
	  string DoName()     { return "D"; }
	};
	
	string D::D2::DoName(){ return d_->DoName(); }
	
  //上面的代码的不足之处
  //(1) 由于提供operator B2&(),较之指针，引用就得特殊对待
  //(2) 将D做为B2使用时，调用者代码必须显示的调用D::AsB2()
        //意味着，在测试程序中，必须将B2 *pb2=&d;修改为B2 *pb2=&d.AsB2();
  //(3) 不能通过dynamic_cast将D转换为B2*。
  
  //你不是经常使用多继承，一旦你使用多继承，那么说明你真的需要多继承
  
  