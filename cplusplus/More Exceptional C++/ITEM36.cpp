ITEM36:��ʼ��
	//1.ֱ�ӳ�ʼ���Ϳ�����ʼ���кβ�ͬ��
	
	//ֱ�ӳ�ʼ��
	U u;
	T t1(u); // calls T::T( U& ) or similar
	//������ʼ��
	T t2 = t1;  // same type: calls T::T( T& ) or similar
	T t3 = u;   // different type: calls T::T( T(u) )
            //  or T::T( u.operator T() ) or similar

  //���׼��:������ʼ����������T t(u),��Ҫ����T t=u;
  
  //2.������������Щʹ��ֱ�ӳ�ʼ������Щʹ�ÿ�����ʼ����
  class T : public S
	{
	public:
	  T() : S(1),             // �����ʼ��
	        x(2) {}           // ��Ա��ʼ��
	  X x;
	};
	
	T f( T t )                // ���ݺ�������
	{
	  return t;               // ����ֵ
	}
	
	S s;
	T t;
	S& r = t;
	
	reinterpret_cast<S&>(t);  // ִ�� reinterpret_cast
	static_cast<S>(t);        // ִ�� static_cast
	dynamic_cast<T&>(r);// ִ�� dynamic_cast
	const_cast<const T&>(t);  // ִ�� const_cast
	
	try
	{
	  throw T();              // �׳��쳣
	}
	catch( T t )              // �����쳣
	{
	}
	
	f( T(s) );                // functional-notation type conversion
	S a[3] = { 1, 2, 3 };     // brace-enclosed initializers
	S* p = new S(4);          // new expression
	
	
	//��������Ĵ�
	class T : public S
	{
	public:
	  T() : S(1),              // base initialization
	        x(2) {}            // member initialization
	  X x;
	};
	
	//����ͳ�Ա��ʼ������ֱ�ӳ�ʼ��
	
	T f( T t )                // passing a function argument
	{
	  return t;               // returning a value
	}
	
	//ֵ�ô��ݺͷ��ض��ǲ��ÿ�����ʼ��
	
	S s;
	T t;
	S& r = t;
	
	reinterpret_cast<S&>(t);  // performing a reinterpret_cast
	dynamic_cast<T&>(r);      // performing a dynamic_cast
	const_cast<const T&>(t);  // performing a const_cast
	
	//Ȧ��: ��Щ�ط���ȫû���漰���¶����ʼ����ֻ�Ǵ��������á�
	
	static_cast<S>(t);        // performing a static_cast
	
	A static_cast uses direct initialization.
	
	try
	{
	  throw T();              // throwing an exception
	}
	catch( T t )              // handling an exception
	{
	}
	
	//�쳣������׳��Ͳ���ʹ�ÿ�����ʼ��
	f( T(s) );              //������ʽ������ת����ֱ�ӳ�ʼ��

	
	
	S a[3] = { 1, 2, 3 };   // �����ŵĳ�ʼ����䣬������ʼ��
	
	Brace-enclosed initializers use copy initialization.
	
	S* p = new S(4);        // new���ʽ ֱ�ӳ�ʼ��
	
	
	
			
			
				