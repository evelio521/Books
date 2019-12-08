ITEM21:δ����ָ���������֮��:ʹ��auto_ptr
	//1.��������쳣��ȫ�������𣿽��ϸ�����Ĵ���Ľ����ģ�
	// Example 21-1
	//
	// In some header file:
	void f( auto_ptr<T1>, auto_ptr<T2> );
	
	// In some implementation file:
	f( auto_ptr<T1>( new T1 ), auto_ptr<T2>( new T2 ) );
	//����auto_ptr֮��ģ���������ִ�в�������(һ)
	//(1)ΪT1�����ڴ�
  //(2)����T1
  //(3)ΪT2�����ڴ�
  //(4)����T2
  //(5)����auto_ptr<T1>
  //(6)����auto_ptr<T2>
  //(7)����f()
  //����Ĵ����ڵ���������Ĳ��κ�һ���������⣬�ϸ�������쳣��Ȼ����
	//����auto_ptr֮��ģ���������ִ�в�������(��)
	//(1)ΪT1�����ڴ�
  //(2)ΪT2�����ڴ�
  //(3)����T1
  //(4)����T2
  //(5)����auto_ptr<T1>
  //(6)����auto_ptr<T2>
  //(7)����f()
  //����Ĵ����ڵ���������Ĳ��κ�һ���������⣬�ϸ�������쳣��Ȼ����
  
	//2.��ʾ���дһ��auto_ptr_new���ߣ������(1)�����⣬���������µ���
	// Example 21-2
		//
		
		// In some header file:
		void f( auto_ptr<T1>, auto_ptr<T2> );
		
		// In some implementation file:
		f( auto_ptr_new<T1>(), auto_ptr_new<T2>() );
		
		//��򵥵ķ������ṩ����һ��ģ��
		// Example 21-2(a): Partial solution
		//
		template<typename T>
		auto_ptr<T> auto_ptr_new()
		{//ֻ����ȱʡ���캯����Ч
		  return auto_ptr<T>( new T );
		}
		//������һ����ȫ���⣬�������Ĵ��벻������Դй©���⣬��Ϊ��û����ֻ������������������֪����
		//�������ύ������
		
		//�뿴������������,�����Ĳ������쳣���������ڴ�й©
		//(1) ����auto_ptr<T1>()
		//(2)	����auto_ptr<T1>()
		
		//���û��ȱʡ���������⣬ʱ�ھ���ͨ����
		// Example 21-2(b): Improved solution
		//
		template<typename T>
		auto_ptr<T> auto_ptr_new()
		{
		  return auto_ptr<T>( new T );
		}
		
		template<typename T, typename Arg1>
		auto_ptr<T> auto_ptr_new( const Arg1& arg1 )
		{
		  return auto_ptr<T>( new T( arg1 ) );
		}
		
		template<typename T, typename Arg1, typename Arg2>
		auto_ptr<T> auto_ptr_new( const Arg1& arg1,
		                          const Arg2& arg2 )
		{
		  return auto_ptr<T>( new T( arg1, arg2 ) );
		}
		
    //һ�����õĽ����������������ʱ��auot_ptr����ֱ�Ž����Ե�����������
    // Example 21-1(b): A simpler solution
		//
		
		// In some header file:
		void f( auto_ptr<T1>, auto_ptr<T2> );
		
		// In some implementation file:
		{
		  auto_ptr<T1> t1( new T1 );
		  auto_ptr<T2> t2( new T2 );
		  f( t1, t2 );
		}
    
    
    //���ԭ��: �ڸ��Զ����ĳ��������ִ��ÿһ����ʾ����Դ
    //����(new)������ͨ��(new)�������Դ�������������߶���(auto_ptr)
