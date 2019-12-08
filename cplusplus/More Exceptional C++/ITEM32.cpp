ITEM32:�ݹ�����
	//1.ʲô�Ǻ���ָ�룿���ʹ������
	
	//����ָ���ǿ������㶯̬��ָ�����ĳ��ԭ�͵ĺ���������:
	// Example 32-1
	//
	
	// Create a typedef called FPDoubleInt for a function
	// signature that takes a double and returns an int.
	//
	typedef int (*FPDoubleInt)( double );
	
	// Use it.
	//
	int f( double ) { /* ... */ }
	int g( double ) { /* ... */ }
	int h( double ) { /* ... */ }
	
	FPDoubleInt fp;
	fp = f;
	fp( 1.1 );     // calls f()
	fp = g;
	fp( 2.2 );     // calls g()
	fp = h;
	fp( 3.14 );    // calls h()
	
		//2.�������д����ôһ�����������ܷ���ָ�������ָ�롣�������ͬ�����Է���
		//һ��ָ�룬ָ���κ�һ������������ͬԭ�͵ĺ��������ֹ�����ʲô�ã�
		
		//Ϊ��ʵ��һ��״̬������ʱ��ÿһ��״̬д��һ�������͹���
		//����״̬��������ͬ��ԭ�ͣ����Ƕ�����һ��ָ�룬ָ����һ����Ҫ���õ�
		//����
		
		//�˴�Ӧ�û���״̬��ͼ
		
		//������һ�����ȼ򻯵Ĵ���Ƭ�Σ���ʾ��������˼��
		// Example 32-2
		//
		StatePtr Start( const string& input );
		StatePtr S2   ( const string& input );
		StatePtr Stop ( const string& input );
		StatePtr Error( const string& input ); // error state
		StatePtr Start( const string& input )
		{
		  if( input == "a" )
		  {
		    return S2;
		  }
		  else if( input == "be" )
		  {
		    return Stop;
		  }
		  else
		  {
		    return Error;
		  }
		}
		//3.�п���д��һ������f()��ʹ�䷵�������ָ�������������ֺ���Ȼ��ʹ����
		//��Ӧ�ú�����
		// Example 32-3
		//
		// FuncPtr is a typedef for a pointer to a
		// function with the same signature as f()
		//
		FuncPtr p = f();    // executes f()
		(*p)();             // executes f()
		//����п��ܣ�����ʾ���ʵ�֣���������ܣ�˵��Ϊʲô��
		
		//�ǵģ��п���ʵ�֣����Ƿ���������ô�Զ��׼�
		
		// Example 32-3(a): ����ĳ��� ����ȷ
		//
		typedef FuncPtr (*FuncPtr)(); // error
		
		// Example 32-3(b): �Ǳ�׼�Ҳ�������ֲ��
		//�Ⲣ����һ���������������������Ҫ�󣬶��Һ�Σ��
		//
		typedef void* (*FuncPtr)();
		
		void* f() { return (void*)f; }  // cast to void*
		FuncPtr p = (FuncPtr)(f());     // cast from void*
		p();
		
		// Example 32-3(c): ���ϱ�׼�Ҿ��п���ֲ��
		// �������к���
		//
		typedef void (*VoidFuncPtr)();
		typedef VoidFuncPtr (*FuncPtr)();
		
		VoidFuncPtr f() { return (VoidFuncPtr)f; }
		                          // cast to VoidFuncPtr
		FuncPtr p = (FuncPtr)f(); // cast from VoidFuncPtr
		p();
		
		//������һ����ȷ����ֲ�Եķ���
		// Example 32-3(d): A correct solution
		//
		class FuncPtr_;
		typedef FuncPtr_ (*FuncPtr)();
		class FuncPtr_
		{
		public:
		  FuncPtr_( FuncPtr p ) : p_( p ) { }
		  operator FuncPtr() { return p_; }
		private:
		  FuncPtr p_;
		};
		
		//���Ժ���Ȼ�������������ʹ��f()
		
		FuncPtr_ f() { return f; } // natural return syntax
		
		int main()
		{
		  FuncPtr p = f();  // natural usage syntax
		  p();
		}
		
		
				
