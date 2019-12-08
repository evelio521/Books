ITEM21:��д���⺯ʽ
	#include <iostream> 
	#include <complex>
	using namespace std;
	class Base
	{
	public:
	  virtual void f( int );
	  virtual void f( double );
	  virtual void g( int i = 10 );
	};
	void Base::f( int )
	{
	  cout << "Base::f(int)" << endl;
	}
	void Base::f( double )
	{
	  cout << "Base::f(double)" << endl;
	}
	void Base::g( int i )
	{
	  cout << i << endl;
	}
	class Derived: public Base
	{
	public:
	  void f( complex<double> );
	  void g( int i = 20 );
	};
	void Derived::f( complex<double> )
	{
	  cout << "Derived::f(complex)" << endl;
	}
	void Derived::g( int i )
	{
	  cout << "Derived::g() " << i << endl;
	}
	void main()
	{
	  Base    b;
	  Derived d;
	  Base*   pb = new Derived;
	  b.f(1.0);
	  d.f(1.0);
	  pb->f(1.0);
	  b.g();
	  d.g();
	  pb->g();
	  delete pb;
	}
	//void main���Ǳ�׼��д�� �������кܴ����ֲ�� ��õ�д��
	  //int main()
	  //int main(int argc,char* argv[])
	//delete pb;�ǲ���ȫ�� ��Ϊû��������������
	//���׼��
	 1.��base class ������������Ϊvirtual
	 2.����Ҫ�ṩһ������ʽ����������̳ж�������ʽͬ��ʱ������㲻�����ڼ̳ж����ĺ���ʽ
	   ����using declaration��ͻΧ
	 3.����Ҫ�ڸı�����ʽ�Ĺ����б��Ԥ�����
	 
	 
	 /*
	  * ����: һ������ʽf() ,��ͬһ��scope���ṩ����һ��ͬ������ʽ��
	  *       ��f()�����У�����������ʵ���������
	  * ��д: һ�����⺯��ʽf(),��derived class�ṩһ��������ͬ�ĺ���ʽ������Ҳ��ͬ
	  * ����: һ������ʽf() ����˼������Χscope���ڲ�scope�ṩһ��ͬ���ĺ�����
	  */
	  
	  //����f()�������أ����صĳ����Ǹ��ݾ�̬����(Base)������,�����Ǹ��ݾ�̬����(derived)
	  
	
