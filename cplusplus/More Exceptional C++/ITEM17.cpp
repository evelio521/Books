ITEM17:���캯��ʧ��֮һ:�����������
	//�����������
	// Example 17-1
		//
		class C : private A
		{
		  B b_;
		};
		//��C�Ĺ��캯���У���β�׽�ӻ������i�����Ա�����캯��
		//���׳����쳣
		
		
		//����function try block������֮��
		// Example 17-1(a): Constructor function try block
		//
		C::C()
		try
		  : A ( /*...*/ )   // optional initialization list
		  , b_( /*...*/ )
		{
		}
		catch( ... )
		{
		  // We get here if either A::A() or B::B() throws.
		
		  // If A::A() succeeds and then B::B() throws, the
		  // language guarantees that A::~A() will be called
		  // to destroy the already-created A base subobject
		  // before control reaches this catch block.
		}

	//�������
	// Example 17-2
	//
	{
	  Parrot p;
	}
  //���������������ں�ʱ��ʼ��
  //�����Ĺ��캯���ɹ�ִ����ϲ���������֮ʱ��Ҳ����˵�������Ƶִﹹ�캯������ĩβ֮ʱ
  //������ɸ���һ��returne���ʱ
  
  //��ʱ������
  //����������������ʼִ��֮ʱ��Ҳ����˵�ִ�����������ʼ��
  
  //�ڶ�������������⣬������ʲô״̬��
  //���������ڿ�ʼ֮ǰ�ͽ���֮�󣬶����״̬��ȫһ����û�ж������
  
  //���������Ĺ��캯���׳�һ���쳣���ǽ���ζ��ʲô��
  //��ζ�Ź���ʧ�ܣ�����û�д��ڹ���������������û�п�ʼ��

  //����C++�Ĺ��캯�����ܽ�����:ֻ���Ƕ���֮һ
  
  (A) ���캯���������أ�������ʵ����
  (B) ���캯���׳��쳣���˳��������δ���ڡ�
