ITEM19:δ������쳣
	//std::uncaught_exception()���ʲô����
	
	//��׼uncaught_exception()�����ṩһ�ַ���������֪����ǰ�Ƿ���һ���쳣�����ڻ״̬��
	
	//�׳��쳣��������������������
	// Example 19-1: The problem
	//
	class X {
	public:
	  ~X() { throw 1; }
	};
	
	void f() {
	  X x;
	  throw 2;
	} // calls X::~X (which throws), then calls terminate()
	//���һ���쳣�Ѿ����ڻ״̬�������ʱ���������׳��쳣���������ֹ���ⲻ�Ǽ�����
	
	
	//�뿴����Ĵ���
	// Example 19-2: The wrong solution
	//
	T::~T()
	{
	  if( !std::uncaught_exception() )
	  {
	    // ... code that could throw ...
	  }
	  else
	  {
	    // ... code that won't throw ...
	  }
	}
	//�������ķ���Ϊʲô����ȷ��
	//����Ĵ��벻�������������������������Ϊ��ʹ���ڿ��԰�ȫ�׳��쳣��ʱ��
	//��Ҳ������������׳��쳣��ִ��·����
	
	// Example 19-2(a): Why the wrong solution is wrong
	//
	U::~U()
	{
	  try
	  {
	    T t;
	    // do work
	  }
	  catch( ... )
	  {
	    // clean up
	  }
	}
	//������쳣�����ڼ䣬���ڶ�ջչ��ʹ��U���󱻴ݻ٣���ôT::~T()������
	//ʹ���������׳��쳣��ִ��·������ʹ��ʱ���԰�ȫ���׳��쳣����ΪT::~T()
	//��֪������������£����Ѿ��ܵ����ⲿU::~U()�����ı���
	
	//��һ�ֲ�ͬ��ʽ�Ĵ��󷽰�
	// Example 19-3: Variant, another wrong solution
	//
	Transaction::~Transaction()
	{
	  if( uncaught_exception() )
	  {
	   RollBack();
	  }
	  else
	  {
	   // ...       }
	}
  //ͬ�����ڶ�ջչ���ڼ䣬���һ�������õ����������õ���Transaction����δ��벻������ȷ�����顣
  // Example 19-3(a): Why the variant wrong solution
	// is still wrong
	//
	U::~U() {
	  try {
	    Transaction t( /*...*/ );
	    // do work
	  } catch( ... ) {
	    // clean up
	  }
	}
  //��֮��Example 19-2�����������������������
  //��ȷ�ķ���
  // Example 19-4: The right solution
	//
	T::~T() /* throw() */
	{
	  // ... code that won't throw ...
	}
	//��һ����ȷ�ķ���
	// Example 19-5: Alternative right solution
	//
	T::Close()
	{
	  // ... code that could throw ...
	}
	
	T::~T() /* throw() */
	{
	  try
	  {
	    Close();
	  }
	  catch( ... ) { }
	}
	//���׼��: ���������쳣�����������׳���д��������ʱ���������Ѿ�����һ��throw()
	          //�쳣�淶һ��
	//���׼��: �����������������һ�����ܻ��׳��쳣�ĺ�����һ��Ҫ��������ð�װ��try/catch block
	          //�У��Է�ֹ�쳣�ӳ���������


  //����uncaught_exception() ��������ò�Ҫʹ��
	


