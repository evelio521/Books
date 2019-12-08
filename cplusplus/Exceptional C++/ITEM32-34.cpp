ITEM32-34:�ӿ�ԭ��
	//*** Example 1 (a) 
	class X { /*...*/ };
	/*...*/
	void f( const X& );
	
	//*** Example 1 (b) 
	class X
	{
	  /*...*/
	public:
	  void f() const;
	};
	
	//�ӿ�ԭ�򣺶���һ����X���������ɺ�����ֻҪͬʱ����
	//1.�ᵽX������
	//��.��Xͬ���ṩ(��Example 1 (a))
	//��ô����X���߼���ɲ��֣���Ϊ�����γ���X�Ľӿ�
	//��ô���ݶ������еĳ�Ա��������X����ɲ��֣���Ϊ
	//��.ÿ����Ա�����������ᵽX����static��Ա������������X* cont ��const X* const��
	//2.ÿ����Ա��������Xͬ���ṩ
	
	
	//*** Example 1 (c) 
		class X { /*...*/ };
		/*...*/
		ostream& operator<<( ostream&, const X& );
		
		//���operator<<��Xͬ���ṩ����ô������X���߼���ɲ���
		
		//class�Ķ���:һ��class������һ�����ݼ�������Щ���ݵĺ���
		
		//�������������
		/*** Example 2 (a) ***/ 
		struct _iobuf { /*...data goes here...*/ };
		typedef struct _iobuf FILE;
		FILE* fopen ( const char* filename,
		              const char* mode );
		int   fclose( FILE* stream );
		int   fseek ( FILE* stream,
		              long  offset,
		              int   origin );
		long  ftell ( FILE* stream );
		     /* etc. */
		//���ǰ������������class��д
		//*** Example 2 (b) 
		class FILE
		{
		public: 
		  FILE( const char* filename,
		        const char* mode );
		~FILE();
		  int  fseek( long offset, int origin );
		  long ftell();
		       /* etc. */
		private:
		  /*...data goes here...*/
		};
		//��Ȼ���ǿ��Խ�����ʵ��һ����Ϊ��Ա������һ����Ϊ�ǳ�Ա����
		//*** Example 2 (c) 
		class FILE
		{
		public:
		  FILE( const char* filename,
		        const char* mode );
		~FILE();
		  long ftell();
		       /* etc. */
		private:
		  /*...data goes here...*/
		};
		int fseek( FILE* stream,
		           long  offset,
		           int   origin );
		
		//�ǲ��ǳ�Ա������������Ĺؼ���ֻҪ���ἰFILE������FILEͬ���ṩ����
		//����FILE����ɲ���
				
				
	//����Koenig Lookup
	//�����������
	//*** Example 3 (a) 
		namespace NS
		{
		  class T { };
		  void f(T);
		}
		NS::T parm;
		int main()
		{
		  f(parm);    // OK: calls NS::f
		}
		//����㴫������һ��class���͵�ʵ�Σ�Ϊ�˲����������������������Ҫ�󲻽�Ҫ������ֲ������������ĳ���ռ䣬��Ҫ����
		//����ʵ�����͵������ռ䡣
		
		//*** Example 3 (b) 
		#include <iostream>
		#include <string>  // this header declares the function
		                   //   std::operator<< for strings
		int main()
		{
		  std::string hello = "Hello, world";
		  std::cout << hello;   // OK: calls std::operator<<
		}
		//���û��Koenig Lookup �������޷��ҵ�operator<<.
		//���Ǳ���дstd::operator<<(std::cout,hello);�� using namespace std;


		//�����������
		//*** Example 4 (a) 
		namespace NS   // this part is
		{             // typically from
		  class T { }; // some header
		}             // file T.h
		void f( NS::T );
		int main()
		{
		  NS::T parm;
		  f(parm);     // OK: calls global f
		}
		//
		//*** Example 4 (b) 
		namespace NS   // typically from
		{             // some header T.h
		  class T { };
		  void f( T ); // <-- new function
		}
		void f( NS::T );
		int main()
		{
		  NS::T parm;
		  f(parm);     // ambiguous: NS::f
		}             //   or global f?
		//�������ռ�����һ�������ƻ��������ռ���Ĵ���

		//*** The Myers Example: "Before" 
		namespace A
		{
		  class X { };
		}
		namespace B
		{
		  void f( A::X );
		  void g( A::X parm )
		  {
		    f(parm);   // OK: calls B::f
		  }
		}//��ܺã����ǵ�����Ĵ������ʱ
		//*** The Myers Example: "After" 
		namespace A
		{
		  class X { };
		  void f( X ); // <-- new function
		}
		namespace B
		{
		  void f( A::X );
		  void g( A::X parm )
		  {
		    f(parm);   // ambiguous: A::f or B::f?
		  }
		}
		
		//��������ṩ��һ���ἰX�ĺ�����������X�����������ռ��ṩĳ������ǩ���غ�ʱ��
		//���þͻ��ж����ԣ�������Ĵ���
		namespace A
		{
		  class X { };
		  ostream& operator<<( ostream&, const X& );
		}
		namespace B
		{
		  ostream& operator<<( ostream&, const A::X& );
		  void g( A::X parm )
		  {
		    cout << parm; // ambiguous: A::operator<< or
		  }              //   B::operator<<?
		}

		
		//�������
		//*** NOT the Myers Example 
		namespace A
		{
		  class X { };
		  void f( X );
		}
		class B      // <-- class, not namespace
		{
		  void f( A::X );
		  void g( A::X parm )
		  {
		    f(parm); // OK: B::f, not ambiguous
		  }
		};
		
		
		//�������������
		//*** Example 5 (a) -- nonvirtual streaming 
		class X
		{
		  /*...ostream is never mentioned here...*/
		};
		ostream& operator<<( ostream& o, const X& x )
		{
		  /* code to output an X to a stream */
		  return o;
		}
		//*** Example 5 (b) -- virtual streaming 
		class X
		{
		  /*...*/
		public:
		  virtual ostream& print( ostream& ) const;
		};
		ostream& X::print( ostream& o ) const
		{
		  /* code to output an X to a stream */
		  return o;
		}
		ostream& operator<<( ostream& o, const X& x )
		{
		  return x.print( o );
		}
		//ѡ��a�ĺô��ǣ������Ե͡�û���麯�����ÿ���
		//ѡ��b�ĺô��ǣ�������ȷ�Ĵ�ӡ��������
		
		//���Ǵӽӿ�ԭ������ ����Ӧ��ѡ��b�������������������
		
		//һЩ��Ȥ��������Ľ��
		//ͨ�������A��B����class������f(A,B)��һ�����ɺ���
		//1.���A��fͬ���ṩ����ôf��A����ɲ��֣�����A������B
		//2.���B��fͬ���ṩ����ôf��B����ɲ��֣�����B������A
		//3.���A��B��f����ͬ���ṩ�ģ���ôfͬʱ��A��B����ɲ��֣�����
		    //A��B��ѭ�����������һ�����ṩ������class��ͬʱ�漰���ߵĲ���
		    //��ô����������涨��ͬʱʹ�á�
		
		
		//���A��B����class�����ң�::��(��)��A�ĳ�Ա����
		//��.��ΪA::g(B)�Ĵ��ڣ�A��������B
		//��.���A��B��ͬ���ṩ�ģ���ô��::��(��)Ҳ��ͬ���ṩ�ġ���Ϊ��::��(��)
		  //ͬʱ�����ἰB����Bͬ���ṩ����ô��::��(��)��B����ɲ��֣�����::��(��)ʹ����һ��
		  //A*����������B����A����ΪAҲ����B����ζ��A,Bѭ��������
		
		
		//*** Example 6 (a) 
		//---file a.h---
		namespace N { class B; }// forward decl
		namespace N { class A; }// forward decl
		class N::A { public: void g(B); };
		//---file b.h---
		namespace N { class B { /*...*/ }; }
		//A���û�������a.h������A��B��ͬ���ṩ�Ĳ�����ѭ�������ġ�B�û�������b.h
		//����A��B����ͬ���ṩ��

								
								
		//�ܽ�
		1.�ӿ�ԭ�򣺶���class X ,���к������������ɺ�����ֻҪͬʱ����
		   a)�ἰX
		   b)��Xͬ���ṩ
		   ��ô������X���߼���ɲ��֣���Ϊ����X�Ľӿ�һ���֡�						
		2.�ǳ�Ա�����ͳ�Ա��������class���߼���ɲ��֣�ֻ������Ա���ȷǳ�Ա������
		  ��ǿ�Ĺ�����ϵ
		3.	�ӿ�ԭ���У���ͬ���ṩ�����õĽ�����:��������ͬ��ͷ�ļ��������ռ��С�				
										
								
