ITEM27:(��)���麯��
	//ʲô�Ǵ��麯��������һ������
	// Example 27-1
	//
	class AbstractClass
	{
	  // declare a pure virtual function:
	  // this class is now abstract
	  virtual void f(int) = 0;
	};
	
	class StillAbstract : public AbstractClass
	{
	  // does not override f(int),
	  // so this class is still abstract
	};
	
	class Concrete : public StillAbstract
	{
	public:
	  // finally overrides f(int),
	  // so this class is concrete
	  void f(int) { /*...*/ }
	};
	
	AbstractClass a;    // error, abstract class
	StillAbstract b;    // error, abstract class
	Concrete      c;    // ok, concrete class
	
 //������һ�����麯������Ϊʲô����Ϊ���ṩ����(������)�������ܸ��������������ɺͳ���
 
 //������������
   /*
    * ���еĻ������������ҪôӦ�������⹲�г�Ա��ҪôӦ���Ƿ����Ᵽ����Ա��
    * �򵥵�˵��������Ϊ�����ȣ�Ҫ��ס����Ӧ�����Ǳ����ʵ������������������������ʵ����
    * ��ô�����Ͳ������ʵ���������Ŀ��ȥ�ṩһ��һ����������������������ֻʣ��
    * ����ѡ��
    * (1) Ҫô������Ҫͨ������ָ����ж�̬ɾ���Ĺ��ܣ���������������������������⹲�г�Ա
    * (2)Ҫô���㲻��Ҫ��һ���ܣ���������£���������Ӧ���Ƿ����⹫�г�Ա--֮�����Ǳ�����Ա��Ϊ�˷�ֹ����
    * ���һ����Ӧ���ǳ����࣬������û�������κδ��麯��������һ������������������ô�����������������Ϊ���麯��
    */
    // Example 27-2(a)
		//
		// file b.h
		//
		class B
		{
		public: /*...other stuff...*/
		  virtual ~B() = 0; // pure virtual destructor
		};
		//��Ȼ���κ����������������������ʽ�ĵ��û�����������������������������ǵö��壬��ʹΪ��
		// Example 27-2(a), continued
		//
		// file b.cpp
		//
		
		B::~B() { /* possibly empty */ }
		
		//������ṩ������壬�㻹�ǿ������������B����������Щ�����಻��ʵ�������Ӷ�û��ʲô�ر��ô�
		
		//���׼��:�������������Ҫô�����⹲�г�ԱҪô�Ƿ����Ᵽ����Ա

   //2.��ȷ��ʹ��ȱʡ��Ϊ
   //���������û�и�дĳ����ͨ���麯�������ͻ�Ĭ�ϵļ̳л����е���Ϊ
   //������ṩһ��Ĭ����Ϊ�����ֲ�������������ô������Ϣ�ļ̳У����������һ��
   //������Ȼ�ṩȱʡ��Ϊʵ�֣����������������������Ҫʹ�������ͱ��������������е���
   // Example 27-2(b)
		//
		class B
		{
		protected:
		  virtual bool f() = 0;
		};
		
		bool B::f()
		{
		  return true;   // this is a good default, but
		}
		                 // shouldn't be used blindly
		
		class D : public B
		{
		  bool f()
		  {
		    return B::f(); // if D wants the default
		  }
		                   // behaviour, it has to say so
		};
		//3.�ṩ������Ϊ
		//��ʱ��������Ҫ���������ṩ������Ϊ��ͬʱ��������໹����
		//��������������һ�ֺ��м�ֵ��Ӧ�ã������˼����:����������
		//������ʵ����Ϊ������ʵ�ֵ�һ������ִ��
		
		// Example 27-2(c)
		//
		class B
		{
		  // ...
		protected virtual bool f() = 0;
		};
		
		bool B::f()
		{
		  // do something general-purpose
		}
		
		class D : public B
		{
		  bool f()
		  {
		    // first, use the base class's implementation
		    B::f();
		
		    // ... now do more work ...
		  }
		};
		
		
				
