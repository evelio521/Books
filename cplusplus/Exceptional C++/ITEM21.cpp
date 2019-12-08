ITEM21:改写虚拟函式
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
	//void main不是标准的写法 ，不具有很大的移植性 最好的写成
	  //int main()
	  //int main(int argc,char* argv[])
	//delete pb;是不安全的 因为没有虚拟析构函数
	//设计准则
	 1.让base class 的析构函数成为virtual
	 2.当你要提供一个函数式，其名称与继承而来函数式同名时，如果你不想遮掩继承而来的函数式
	   请用using declaration来突围
	 3.绝不要在改变虚拟式的过程中变改预设参数
	 
	 
	 /*
	  * 重载: 一个函数式f() ,在同一个scope中提供另外一个同名函数式。
	  *       当f()被呼叫，编译器根据实际情况调用
	  * 重写: 一个虚拟函数式f(),在derived class提供一个名字相同的函数式，参数也相同
	  * 遮掩: 一个函数式f() ，意思是在外围scope的内层scope提供一个同名的函数。
	  */
	  
	  //其中f()属于重载，重载的程序是根据静态类型(Base)决定的,而不是根据静态类型(derived)
	  
	
