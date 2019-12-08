ITEM30:FAST PIMPL����
	
	//��׼��malloc��new���ÿ����ܴ�
	// Attempt #1 
	//
	// file y.h
	#include "x.h"
	class Y
	{
	  /*...*/
	  X x_;
	};
	
	// file y.cpp
	Y::Y() {}
	
  //���class Y��������Ҫclass X �������Ѿ��ɼ���Ϊ�˱����������
  // Attempt #2 
	//
	// file y.h
	class X;
	class Y
	{
	  /*...*/
	  X* px_;
	};
	
	// file y.cpp
	#include "x.h"
	Y::Y() : px_( new X ) {}
	Y::~Y() { delete px_; px_ = 0; }

	//��ܺõ�������X�ģ����ǵ�Y���㷺ʹ��ʱ����̬�ڴ�Ŀ������������ܡ�
	//������һ�����ƺܺõĽ����������ʵ������
	// Attempt #3 
	//
	// file y.h
	class Y
	{
	  /*...*/
	  static const size_t sizeofx = /*some value*/;
	  char x_[sizeofx];
	};
	
	// file y.cpp
	#include "x.h"
	Y::Y()
	{
	  assert( sizeofx >= sizeof(X) );
	  new (&x_[0]) X;
	}
	Y::~Y()
	{
	  (reinterpret_cast<X*>(&x_[0]))->~X();
	}
	
	
	//FAST PIMPL����
	// file y.h 
	class Y
	{
	  /*...*/
	  struct YImpl;
	  YImpl* pimpl_;
	};
	
	// file y.cpp
	#include "y.h"
	struct Y::YImpl
	{
	  /*...private stuff here...*/
	  static void* operator new( size_t )   { /*...*/ }
	  static void  operator delete( void* ) { /*...*/ }
	};
	X::X() : pimpl_( new XImpl ) {}
	X::~X() { delete pimpl_; pimpl_ = 0; }
	
	//С��
	//�����Ȼ�ã���Ҳ��Ҫ����FAST PIMPL����õ�����ѵ��ڴ�����ٶȡ����Ǳ����Ǵ���:
	//ά����Щ����ͨ�����¿ռ�Ч���½�����Ϊ��ͨ�������ɸ������Ƭ��
		
	//����֤��ȷʵ��Ҫ��ʱ����ʹ�á�
			