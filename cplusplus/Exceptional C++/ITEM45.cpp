ITEM45:BOOL
	//��bool���͵ļ���ʵ�ַ�ʽ�����Ƕ�����ȱ�ݵ�
	
	//1.typedef
	// Option 1: typedef 
	//
	typedef int bool;
	const bool true  = 1;
	const bool false = 0;
	//�����������ǲ�������
	// file f.h 
	void f( int  ); // ok
	void f( bool ); // ok, redeclares the same function
	// file f.cpp
	void f( int  ) { /*...*/ }  // ok
	void f( bool ) { /*...*/ }  // error, redefinition
	//2.#define
  // Option 2: #define 
	//���ַ����к�������������ͬ�����⣬�����ƻ��� #define
	#define bool  int
	#define true  1
	#define false 0
	//3.enum
	// Option 3: enum 
	//
	enum bool { false, true };
	//�������أ������������ʽ�������Զ�����ת��
	bool b; 
	b = ( i == j );//������������
 //4.class
 	class bool 
	{
	public:
	  bool();
	  bool( int );            // to enable conversions from
	  bool& operator=( int ); //  conditional expressions
	  //operator int();   // questionable!
	  //operator void*(); // questionable!
	private:
	  unsigned char b_;
	};
	const bool true ( 1 );
	const bool false( 0 );
	//����ת��������������Ϳ��Թ�������Щ����ԭ��
	//1.�����Զ�����ת�������ؽ����bool�ĳ�ͻ
	//2.���ʹ���Զ�����ת����bool���͵����ݻ�Ӱ�쵽�������أ����������κ�һ�����͵���ʽ���캯�����Զ�����ת��һ��
	//3.����ת����int��void*���Ƶ����ͣ�bool�������в���������
	//4.�����ʹ���Զ�ת�����ͣ���ôbool���ͽ��޷���ʹ���������ж���
	bool b; 
	/*...*/
	if( b ) // error without an automatic conversion to
	{       // something like int or void*
	    /*...*/
	}


	
