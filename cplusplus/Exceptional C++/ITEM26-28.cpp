ITEM26-28:�����ڵİ�ȫ��
	//���������Ա������ͷ�ļ�����ʵ��Ӧ�õĶ࣬�⽫����Ӱ���������ʱ�䡣
	 #include "a.h" //class A
	 #include "b.h" //class B
	 #include "c.h" //class C
	 #include "d.h" //class D
	 //ע��ֻ��A��C�����⺯��
	 #include<iostream>
	 #include<ostream>
	 #include<sstream>
	 #include<list>
	 #include<string>
	 
	 class X : public A{
	 	
	 	public:
	 		X (const &);
	 		D Function1(int char*);
	 		D Function1(int ,C);
	 		B& Function2(B);
	 		void Function3(std::wostringstream&);
	 		std::ostream& print(std::ostream&)const;
	 	private:
	 		std::string name_;
	 		std::list<C> clist_;
	 		D            d_;
	 		
	};
	std::ostream& print(std::ostream& os,const X& x){
		
		return x.print(os);
	}
	
	class Y : private B{
	public;
		C Function4(A);
	private:
		std::list<std::wostringstream*> alist_;
	};
	
	//���ǿ���ֱ��ȥ����ͷ�ļ���
	1.iostream ������Ȼ�õ���������û���õ�����ʵ�ʶ���
	2.ostream��sstream,����Ĳ����ͷ������ͱ�ǰ�������ǿ��Եģ�ֻ��Ҫiosfwd�Ϳ�����
	
	//���ǲ���ȥ����ͷ�ļ�
	1. a.h
	2. b.h
	3. c.h
	4. d.h
	
	//���ǿ���ͨ��X��Yʹ��pimp_�ķ���ȥ�� d.h  list string  c.h
	
	//��Ҫ������:��ʹostreamû�б����壬������operator<<Ҳ������Ȼ�����������ԣ���ʹ��ostream������
	//������Ϊ��ֻ�ڵ��ó�Ա������ʱ�����������Ҫ��Ӧ�Ķ��壬���������һ�����󲢽��䵱����
	//��������ʽ�Ĳ����������κ��������������ʱ���㲢����Ҫ�ú����Ķ���
	
	//B��Y��private���࣬����Bû���麯�������b.h�ǿ���ȥ���ġ���һ����Ҫ��ԭ��������ʹ��private�̳�
	//�Ǿ����������⺯��������Ӧ����Yӵ��һ��B�ĳ�Ա��Ҫȥ��b.h ����Ӧ����Y���������ΪB�ĳ�Ա������Y�����ص�pimp_����
	
	//X��Y������֮��û���κι�ϵ������������ٿ��԰�X��Y�Ķ���ֱ�ŵ�������ͬ��ͷ�ļ���ȥ
	//Ϊ�˲�Ӱ���еĴ����ʹ�ã����ǻ�Ӧ�ð����е�ͷ�ļ���Ϊһ��������������x.h ��y.h
	//����������ٿ�����y.h����#include "a.h" ��Ϊ������ֻ��A���������Ĳ������ͣ�����ҪA�Ķ��塣
	
	 
	 //�Ľ���Ĵ���
	 
	 //x.h
	 #include "a.h"
	 #include<iosfwd>
	 
	 class C;
	 class D;
	 
	 class X : public A{
	 	
	 	public:
	 		X (const &);
	 		D Function1(int char*);
	 		D Function1(int ,C);
	 		B& Function2(B);
	 		void Function3(std::wostringstream&);
	 		std::ostream& print(std::ostream&)const;
	 	private:
	 		class XImpl* pimpl_;
	 		
	};
	inline std::ostream& print(std::ostream& os,const X& x){
		//���ﲻ��Ҫostream �Ķ���
		return x.print(os);
	}
	
	//y.h û�� #include
	class A ;
	class C;
	class Y{
		public:
			C Function4(A);
		private:
			C YImpl* pimp_;
	};
	
	//got007.h ��Ϊ�����������#include ��ͨ��x.h������������#include
	#include "x.h"
	#include "y.h"
	
	//got007.cpp
	
	struct XImpl{//������ʱ����class ���������struct
		
		std::string name_;
		std::list<C> clist_;
		D          d_;
	};
	struct YImpl{//������ʱ����class ���������struct
		
		
		std::list<std::wostringstream*> alist_;
		B          b_;
	};
	//������Xʹ����ֻ��Ҫ#include����a.h ��iosfwd.Y��ʹ����ֻ��Ҫ����a.h ��iosfwd����ʹ�������´������y.h��ȥ��got007.h,Ҳ���ö��#include
	
	
	
	
	
	//Ӣ�İ���е㲻ͬŶ
	//  x.h: original header 
	//
	#include <iostream>
	#include <ostream>
	#include <list>
	// None of A, B, C, D or E are templates.
	// Only A and C have virtual functions.
	#include "a.h"  // class A
	#include "b.h"  // class B
	#include "c.h"  // class C
	#include "d.h"  // class D
	#include "e.h"  // class E
	class X : public A, private B
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  std::list<C> clist_;
	  D            d_;
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	
  //1.Remove iostream.
	//2.Replace ostream with iosfwd.
	//3.Replace e.h with a forward declaration.

	//�������ϸı�����
	//  x.h: sans gratuitous headers 
	//
	#include <iosfwd>
	#include <list>
	// None of A, B, C or D are templates.
	// Only A and C have virtual functions.
	#include "a.h"  // class A
	#include "b.h"  // class B
	#include "c.h"  // class C
	#include "d.h"  // class D
	class E;
	class X : public A, private B
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  std::list<C> clist_;
	  D            d_;
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	
	//We had to leave a.h and b.h. We couldn't get rid of these because X 
	//inherits from both A and B, and you always have to have full definitions
	// for base classes so that the compiler can determine X's object size, 
	//virtual functions, and other fundamentals. (Can you anticipate how to
	// remove one of these? Think about it: Which one can you remove, and why/how? 
	//The answer will come shortly.)

  //We had to leave list, c.h, and d.h. We couldn't get rid of these right 
  //away because a list<C> and a D appear as private data members of X.
  // Although C appears as neither a base class nor a member, it is being
  // used to instantiate the list member, and most current compilers require
  // that when you instantiate list<C>, you are able to see the definition of C. 
  //(The standard doesn't require a definition here, though, so even if the 
  //compiler you are currently using has this restriction, you can expect the 
  //restriction to go away over time.)


  //ʹ��pimp_֮��
  //  x.h: after converting to use a Pimpl 
	//
	#include <iosfwd>
	#include "a.h"  // class A (has virtual functions)
	#include "b.h"  // class B (has no virtual functions)
	class C;
	class E;
	class X : public A, private B
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  struct XImpl;
	  XImpl* pimpl_;
	    // opaque pointer to forward-declared class
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	//  Implementation file x.cpp 
	//
	struct X::XImpl
	{
	  std::list<C> clist_;
	  D            d_;
	};
	
	
	//  x.h: after removing unnecessary inheritance 
	//
	#include <iosfwd>
	#include "a.h"  // class A
	class B;
	class C;
	class E;
	class X : public A
	{
	public:
	     X( const C& );
	  B  f( int, char* );
	  C  f( int, C );
	  C& g( B );
	  E  h( E );
	  virtual std::ostream& print( std::ostream& ) const;
	private:
	  struct XImpl;
	  XImpl* pimpl_; // this now quietly includes a B
	};
	inline std::ostream& operator<<( std::ostream& os, const X& x )
	{
	  return x.print(os);
	}
	
	
	
	//�����׼
	1.��װ�͸���
	2.���������ʱ��Ӧ�ñ��Ⱪ¶��˽�г�Ա
	3.Ӧ��ʹ��һ������struct XxxxImpl* impl_ �Ĳ�͸����ָ�����洢˽�г�Ա(����״̬�����ͳ�Ա����)
	  class Map{
	  	private:
	  		struct MapImpl* impl_;
	  };
		
		
