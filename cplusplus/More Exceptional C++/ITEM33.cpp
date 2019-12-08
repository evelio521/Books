ITEM33:ģ��Ƕ�׺���
	//1.ʲô��Ƕ����?����ʲô�ô���
	//Ƕ������һ����������һ�����е��࣬����
	// Example 33-1: Nested class
	//
	class OuterClass
	{
	/*...public, protected, or private...*/:
	  class NestedClass
	  {
	    // ...
	  };
	
	  // ...
	  };
	//����֯����Ϳ��Ʒ��ʷ���Ȩ�޺ʹ�����ϵ���棬Ƕ�����кܶ��ô���
	//�����е���������һ����Ƕ����Ҳ���ط��ʹ�������NestedClass
	//������Ϊpublic����ô�κ��ⲿ������Գ�֮ΪOuterClass::NestedClass��
	//ͨ��Ƕ������˽��ʵ��ϸ�ڡ�����ᱻ����Ϊprivate�����NestedClass
	//Ϊprivate����ôֻ��OuterClass�ĳ�Ա����Ԫ����ʹ��NestedClass
	
	//ʲô�Ǿֲ��ࣿ��ʲô�ô���
	//�ֲ�����һ��������һ��������Χ�ڵ���--�κκ����������ǳ�Ա�����������ɺ���������
	// Example 33-2: Local class
	//
	int f()
	{
	  class LocalClass
	  {
	    // ...
	  };
	
	  // ...
	};
	//ֻ��f()���ڲ�����֪��LocalClass���ܹ�ʹ��������LocalClass
	//��f()���ڲ�ʵ��ϸ�ڣ������Զ��Ӧ�ù��������������ʱ��������ʾ����ֵ
	
	//��ʹ�÷Ǿֲ��������ĵط�������ʹ�þֲ��࣬���Ǿֲ����δ�������಻����Ϊ
	//ģ�����ʹ�á�
	
	template<class T>
	class X { /* ... */ };
	void f()
	{
	  struct S { /* ... */ };
	  X<S> x3;  // error: local type used as
	              //  template-argument
	    X<S*> x4; // error: pointer to local type
	              //  used as template-argument
	  }
	
		 //3.C++��֧��Ƕ�׺�����Ҳ��˵���ǲ���д������Ĵ���
		// Example 33-3
		//
		int f( int i )
		{
		  int j = i*2;
		
		  int g( int k )  // not valid C++
		  {
		    return j+k;
		  }
		
		  j += 4;
		
		  return g( 3 );
		}
		//Ҫ�����������⣬������˻����������
		// Example 33-3(a): Na?ve "local function object"
		// approach (doesn't work)
		//
		int f( int i )
		{
		  int j = i*2;
		
		  class g_
		  {
		  public:
		    int operator()( int k )
		    {
		      return j+k;   // error: j isn't accessible
		    }
		  } g;
		
		  j += 4;
		
		  return g( 3 );
		}
		//��������ȱ���ǣ��ֲ������ܷ�����Χ�����ı���
		
		//��һ�����ԣ������������б�����ָ��������ṩ������ֲ���
		// Example 33-3(b): Na?ve "local function object plus
		// references to variables" approach (complex,
		// fragile)
		//
		int f( int i )
		{
		  int j = i*2;
		
		  class g_
		  {
		  public:
		    g_( int& j ) : j_( j ) { }
		
		    int operator()( int k )
		    {
		      return j_+k;  // access j via a reference
		    }
		
		  private:
		    int& j_;
		  } g( j );
		
		  j += 4;
		
		  return g( 3 );
		}
		//����������У���ֻ����ǿ���У����������������������
		//׼ȷ��˵������ֻ�ܽ�������һ�ַǳ��ֶΡ�����Ҫ����һ������
		//���ǽ�Ҫ���ĸ��Ķ�
		//(1) �����������
		//(2) Ϊg_����һ����Ӧ��˽�����ó�Ա
		//(3) Ϊg_����һ����Ӧ�Ĺ��캯������
		//(4) Ϊg::g_()����һ����Ӧ�Ĺ��캯������
		
		//������һ�����и��Ƶķ���
		// Example 33-3(c): A better solution
		//
		int f( int i )
		{
		  class g_
		  {
		  public:
		    int j;
		
		    int operator()( int k )
		    {
		      return j+k;
		    }
		  } g;
		
		  g.j = i*2;
		  g.j += 4;
		  return g( 3 );
		}
		
		//��ӽ���ȷ�ķ���
		// Example 33-3(d): Nearly there!
		//
		int f( int i )
		{
		  // Define a local class that wraps all
		  // local data and functions.
		  //
		  class Local_
		  {
		  public:
		    int j;
		
		    // All local functions go here:
		    //
		    int g( int k )
		    {
		      return j+k;
		    }
		    void x() { /* ... */ }
		    void y() { /* ... */ }
		    void z() { /* ... */ }
		  } local;
		
		  local.j = i*2;
		  local.j += 4;
		
		  local.x();
		  local.y();
		  local.z();
		
		  return local.g( 3 );
		}
		
		//һ�������ļ����������Եķ���
		// Example 33-3(e): A complete and nicely
		// extensible solution
		//
		class f
		{
		  int  retval; // f's "return value"
		  int  j;
		  int  g( int k ) { return j + k; };
		  void x() { /* ... */ }
		  void y() { /* ... */ }
		  void z() { /* ... */ }
		
		public:
		  f( int i )   // original function, now a constructor
		    : j( i*2 )
		  {
		    j += 4;
		    x();
		    y();
		    z();
		    retval = g( 3 );
		  }
		  operator int() const // returning the result
		  {
		    return retval;
		  }
		};
		
		//ע������������Ժ���������Ϊ��Ա����������f()�������ɺ���
		//���ǳ�Ա��������������f()��дһ��Ƕ�׺���g(),����
		// Example 33-4: �ⲻ�ǺϷ���C++���룬������ʾ
		//               �����ǵ���Ҫ��һ���ֲ���������
		//                ��һ����Ա����֮��
		//
		class C
		{
		  int data_;
		
		public:
		  int f( int i )
		  {
		    // a hypothetical nested function
		    int g( int i ) { return data_ + i; }
		
		    return g( data_ + i*2 );
		  }
		};
		
		//Ҫ��������Ĺ�ϵ�����ǿ��Խ�f()���һ���ࡣ��33-3(e)����
		//ֻ������33-3(e)���Ǹ���ʵ��ȫ�ֿռ䣬����������һ��Ƕ����
		//����Ҫͨ����������������
		// Example 33-4(a):�����Ҽ���������
		//                 �Ľ���������������ڳ�Ա����
		//
		class C
		{
		  int data_;
		  friend class C_f;
		public:
		  int f( int i );
		};
		
		class C_f
		{
		  C*  self;
		  int retval;
		  int g( int i ) { return self->data_ + i; }
		
		public:
		  C_f( C* c, int i ) : self( c )
		  {
		    retval = g( self->data_ + i*2 );
		  }
		
		  operator int() const { return retval; }
		};
		
		int C::f( int i ) { return C_f( this, i ); }
		
		//���ԭ��: �������������⸴����ƣ�������������
				
					
										
												
														
																
