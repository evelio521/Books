ITEM28:�ܿصĶ�̬
	//������Ĵ���
	class Base
	{
	public:
	  virtual void VirtFunc();
	  // ...
	};
	
	class Derived : public Base
	{
	public:
	  void VirtFunc();
	  // ...
	};
	
	void SomeFunc( const Base& );
	//���⻹����������f1()��f2()�����ǵ�Ŀ���ǣ�����f1()�ڽ���Base����ĵط�
  //��̬ʹ��Derived���󣬵���ֹ�������к�������(f2())������
  void f1()
	{
	  Derived d;
	  SomeFunc( d ); // works, OK
	}
	void f2()
	{
	  Derived d;
	  SomeFunc( d ); // we want to prevent this
	}
  //��ʾ��δﵽ��һЧ��
  
   //����֮�����ܹ��ڽ���Base����ĵط���̬��ʹ��Derived����ԭ������Derived
   //��Base���м̳С��෴���Derived��Base˽�м̳У���ô����û�д�����Զ�̬�Ľ�
   //Derived��ΪBaseʹ�á�֮����˵������ԭ������:���Դ����Է���Derived��˽�г�Ա��
   //�����ǿ��Է���Derived��˽�л��࣬������Զ�̬�Ľ�Derived����ΪBaseʹ�ã�������˵
   //ֻ��Derived�ĳ�Ա�������з���Ȩ��Ȼ��ͨ��C++����Ԫ���ԣ����ǿ��Խ����Ƶķ���Ȩ����
   //�������ⲿ�����С�
   
   
   //��Ҫ�õ���Ŀ�е�Ч��
    class Derived : private Base
		{
		public:
		  void VirtFunc();
		  // ...
		  friend void f1();
		};
		
