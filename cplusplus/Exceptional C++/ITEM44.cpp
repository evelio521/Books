ITEM44:����ת��
	class  A { public: virtual ~A(); /*...*/ }; 
	A::~A() { }
	
	class B : private virtual A  { /*...*/ };
	
	class C : public  A          { /*...*/ };
	
	class D : public B, public C { /*...*/ };
	
	A a1; B b1; C c1; D d1;
	const A  a2;
	const A& ra1 = a1;
	const A& ra2 = a2;
char c;


	//C++����ת��
	//����ļ�More Effective C++ Rule 2
	//1.const_cast 
  //2.dynamic_cast//������C�������Ӧ
  //3.reinterpret_cast
  //4.static_cast


  //����ÿһ����䣬���·��д�����������·���ǲ���ȷ��
		 void f() 
		{
		  A* pa; B* pb; C* pc;
		  pa = (A*)&ra1;//pa = const_cast<A*>(&ra1); 
	    pa = (A*)&a2;//��һ���޷�ʹ����ת��
		  pb = (B*)&c1;// pb = reinterpret_cast<B*>(&c1); 
		  pc = (C*)&d1;//�����C���Ǵ���ģ���C++����Ҫת��pc = &d1;
		  
		}

   //��������ÿһ��C++ת�ͱ�д������ȷ��
   
   //����ע�⣬���ǲ���֪���������и�������ʱ��ӵ�����⺯��������漰ת�͵���
   //���������⺯������ô��dynamic_cast��ת���Ǵ����
   void g() 
	{
	  unsigned char* puc = static_cast<unsigned char*>(&c);
	  signed char* psc = static_cast<signed char*>(&c);
	  //���������Ӧʹ��reinterpret_cast ��ԭ����:char .signed char�Լ�
	  //unsigned char������������ͬ�������������͡���������֮�������ʽת��
	  //��������֮���ǻ�����ϵ�ģ���Ȼָ��Ҳ�ǻ�����ϵ��
	
	  void* pv = static_cast<void*>(&b1);
	  B* pb1 = static_cast<B*>(pv);
	 //�����䶼�������ǵ�һ���ת���ǲ���Ҫ�ģ���������һ������ָ�뵽void*��ʽת��
	  B* pb2 = static_cast<B*>(&b1);
	//���Ҳ��������ת���ǲ���Ҫ�ģ��������Ѿ���һ��B*
	  A* pa1 = const_cast<A*>(&ra1);
	  //��һ����ǺϷ��ģ�����ʹ��ת��ȥ����������Ǳ�ڲ����������֡�����ͨ������ʹ��mutable�ؼ������
	  A* pa2 = const_cast<A*>(&ra2);
		//���������ָ�뱻�����Զ���ʩ��д�����������δ������Ϊ����Ϊa2��const object�������ֻ���洢��
	  B* pb3 = dynamic_cast<B*>(&c1);
		//������һ�佫pb3����ΪNULL����Ϊc1��������һ��B����
	  A* pa3 = dynamic_cast<A*>(&b1);
		//������Ϊb1����һ��A(B����private������A��)
	  B* pb4 = static_cast<B*>(&d1);
		//������û�б�Ҫ
	  D* pd = static_cast<D*>(pb4);
		//�����������Ϊ������Ҫһ��dynamic_cast���ǾͲ����ˡ���Ϊ��Ŀ����֪��ʱ������ת�Ϳ����Ǿ�̬�ġ�
		pa1 = dynamic_cast<A*>(pb2);
		//�����㲻��ʹ��dynamic_cast��һ��ָ��B�����ָ��ת��Ϊָ��A�����ָ�룬��ΪB
		//����private������A
	  pa1 = dynamic_cast<A*>(pb4);
	  //��ȷ
	
	  C* pc1 = dynamic_cast<C*>(pb4);
	  //��ȷ��
	  C& rc1 = dynamic_cast<C&>(*pb2);
	  //����pb2��������ľ���һ��C��dynamic_cast������ָ��ת��ʧ��ʱ������null,����û��null reference 
	  //����˵������˵�һ����������ʧ��ʱ���޷�����null reference�������׳��쳣��û�б�İ취
	  }

