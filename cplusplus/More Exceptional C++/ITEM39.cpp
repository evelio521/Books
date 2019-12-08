ITEM39:���ֿռ䡣֮һ:using������usingָ��
	
	//ʲô��using������using����?���ʹ������?�������ӡ�����
	//�����е���һ����˳�����?
	
	//using����Ϊʵ����������һ�����ֿռ��е����ƴ���һ������ͬ��ʡ�
	//Ϊ�����غ����ƽ�������Ҫ��using������������ʽ����������һ��
	// Example 39-1(a)
	//
	namespace A
	{
	  int f( int );
	  int i;
	}
	
	using A::f;
	
	int f( int );
	
	int main()
	{
	  f( 1 );     // ����: A::f() or ::f()?
	
	  int i;
	  using A::i; // ����, �ظ����� (����
	              //  д������int iһ��)
	}
	//ֻ���Ѿ����ֹ����������ƣ�using�������ܽ�������
	// Example 39-1(b)
	//
	namespace A
	{
	  class X {};
	  int Y();
	  int f( double );
	}
	using A::X;
	using A::Y; // ���� A::Y(), ���� A::Y
	using A::f; //  A::f(double), �� A::f(int)
	
	namespace A
	{
	  class Y {};
	  int f( int );
	}
	
	int main()
	{
	  X x;      // ��ȷ, X �� A::Xͬ���
	
	  Y y;      // ����, A::Y ���ɼ� ��Ϊ
	            //  using��������������Ҫ
	            //  ��ʵ������֮ǰ
	
	  f( 1 );   // ����͵͵ʹ����һ����ʽת����
	            //  �����õ��� A::f(double),������
	            //  A::f(int), ��Ϊ��using����֮ǰ
	            //  ֻ��ͷһ�� A::f() �������ֹ�
	             //  seen at the point of the using-
	             //  declaration for the name A::f
	}
	
	//����usingָ���һ�����ֿռ��е��������ƶ�����ʹ����usingָ�����ڵĿռ䣬
	//��using������ͬ��usingָ��Ὣ������usingָ��֮ǰ��֮�����е����ƶ������
	//������Ȼ����ʹ��ĳ������֮ǰ���Ǹ����Ʊ�����ֹ����С�
	// Example 39-1(c)
	//
	namespace A
	{
	  class X {};
	  int f( double );
	}
	void f()
	{
	  X x;      // OK, X is a synonym for A::X
	  Y y;      // error, no Y has been seen yet
	  f( 1 );   // OK, calls A::f(double) with parameter promotion
	}
	
	using namespace A;
	
	namespace A
	{
	  class Y {};
	  int f( int );
	}
	
	int main()
	{
	  X x;      // OK, X is a synonym for A::X
	  Y y;      // OK, Y is a synonym for A::Y
	  f( 1 );   // OK, calls A::f(int)
	}
	
	
		
