ITEM25:ģ���̳�
	//�����������
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
	
	//������MI��дһ��������ȼ۵���D�������ڲ�ʹ�ö�̳е�����£���ʾ�����ҵ�
	//����Ĩ�ǽ������ķ�������D�õ���ͬ��Ч���Ϳ����ԣ�ͬʱ��Ҫ����Ե����ߴ���ľ䷨
	//�����޸ģ���������
	
	//���ȿ����������������Ȼ��ʼ���
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

  //����ķ��������ǵ�Ҫ��ǳ��ӽ�
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
	
  //����Ĵ���Ĳ���֮��
  //(1) �����ṩoperator B2&(),��ָ֮�룬���þ͵�����Դ�
  //(2) ��D��ΪB2ʹ��ʱ�������ߴ��������ʾ�ĵ���D::AsB2()
        //��ζ�ţ��ڲ��Գ����У����뽫B2 *pb2=&d;�޸�ΪB2 *pb2=&d.AsB2();
  //(3) ����ͨ��dynamic_cast��Dת��ΪB2*��
  
  //�㲻�Ǿ���ʹ�ö�̳У�һ����ʹ�ö�̳У���ô˵���������Ҫ��̳�
  
  