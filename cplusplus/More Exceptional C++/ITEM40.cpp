ITEM40:���ֿռ䡣֮��:Ǩ�㵽���ֿռ�
	
	//�������ڿ���һ���������д������Ŀ�����е�.h��.cpp�ļ�����ǧ����
	//��ʱ����ĿС��Ҫ�����������������°汾������汾�ı�����֧�����ֿռ䣬��׼
	//������й���Ҳ���������ֿռ�std�С����ҵ�������˳Ӧ��׼������Ҳ�и�����
	//�ᵼ�����д��벻��ͨ�����롣����û���㹻��ʱ��ȥ��ϸ����ÿ���ļ�
	//����ν����������أ�
	
	//��ȫ��Ч��Ǩ�㵽���ֿռ�
	//����׼���������ֿռ�std�У������������Ŀ�У��������ε�ʹ��std::���ƽ��޷�����ɹ�
	
	//���д����ȥ����ͨ������
	// Example 40-1: This used to work
	//
	#include <iostream.h>  // ��׼��̨ǰ��ͷ�ļ�
	
	int main()
	{
	  cout << "hello, world" << endl;
	}
	//���ڣ�Ҫô��ѡ��д����Щ���ƴ�����std��
	// Example 40-2(a): Option A, ��ȷָ��һ��
	//
	#include <iostream>
	
	int main()
	{
	  std::cout << "hello, world" << std::endl;
	}
	//Ҫôʹ��using����Ҫ��std�������뵽��ǰ�ռ�
	// Example 40-2(b): Option B, write using-declarations
	//
	#include <iostream>
	
	using std::cout;
	using std::endl;
	
	int main()
	{
	  cout << "hello, world" << endl;
	}
	//Ҫô�򵥵�ʹ��һ��using ָ�������std�����������뵽��ǰ�ռ�
	// Example 40-2(c): Option C, write using-directives
	//
	#include <iostream>
	
	int main()
	{
	  using namespace std; // or this can go at file scope
	  cout << "hello, world" << endl;
	}
	//Ҫô���ۺ����Ϸ���
	
	
	//�õĳ��ڷ��������׼������Ӧ����ѭһ�¹���
	//����1:���Բ�Ҫ��ͷ�ļ���ʹ��usingָ�usingָ�����Ⱦ���ֿռ䣬��Ϊ����������
	//���������ƣ�������������ǲ���Ҫ�ģ�����Ҫ����һ�����֣����ֳ�ͻ�Ϳ��ܼӴ�
	//����2:���Բ�Ҫ��ͷ�ļ���ʹ��using������
	//����3:��ʵ���ļ��У����Բ�Ҫ��#includeָ��֮ǰʹ��using������usingָ��
	//����4:��ʹ��Cͷ�ļ�ʱ�������·���#include<cheader> �������þɷ���#include<header.h>
	
	//һ������������
	// Example 40-3(a): û�����ֿռ��ԭʼ����
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines Y
	#include <deque.h>
	#include <iosfwd.h>
	
	ostream& operator<<( ostream&, const Y& );
	Y operator+( const Y&, int );
	int f( const deque<int>& );
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines Z
	#include <ostream.h>
	
	ostream& operator<<( ostream& o, const Y& y )
	{
	  // ... uses Z in the implementation ...
	  return o;
	}
	
	Y operator+( const Y& y, int i )
	{
	  // ... uses another operator+() in the implementation...
	  return result;
	}
	
	int f( const deque<int>& d )
	{
	  // ...
	}
	
	//һ���õĳ��ڷ���
	// Example 40-3(b): A good long-term solution
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines Y
	#include <deque>
	#include <iosfwd>
	
	std::ostream& operator<<( std::ostream&, const Y& );
	Y operator+( const Y&, int i );
	int f( const std::deque<int>& );
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines Z
	#include <ostream>
	using std::deque;   // "using" appears AFTER all #includes
	using std::ostream;
	using std::operator+;
	// or, "using namespace std;" if that suits you
	ostream& operator<<( ostream& o, const Y& y )
	{
	  // ... uses Z in the implementation ...
	  return o;
	}
	
	Y operator+( const Y& y, int i )
	{
	  // ... uses another operator+() in the implementation...
	  return result;
	}
	
	int f( const deque<int>& d )
	{
	  // ...
	}
	
	//һ������ô�õĳ��ڷ���
	// Example 40-3(c): Bad long-term solution
	// (or, Why to never write using-declarations
	// in headers, even within a namespace)
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines MyProject::Y and adds
	                //  using-declarations/directives
	                //  in namespace MyProject
	#include <deque>
	#include <iosfwd>
	namespace MyProject
	{
	  using std::deque;
	  using std::ostream;
	  // or, "using namespace std;"
	
	ostream& operator<<( ostream&, const Y& );
	Y operator+( const Y&, int );
	int f( const deque<int>& );
	}
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines MyProject::Z and adds
	                //  using-declarations/directives
	                //  in namespace MyProject
	  // error: potential future name ambiguities in
	  //        z.h's declarations, depending on what
	  //        using-declarations exist in headers
	  //        that happen to be #included before z.h
	  //        in any given module (in this case,
	  //        x.h or y.h may cause potential changes
	  //        in meaning)
	#include <ostream>
	
	namespace MyProject
	{
	  using std::operator+;
	
	  ostream& operator<<( ostream& o, const Y& y )
	  {
	    // ... uses Z in the implementation ...
	    return o;
	  }
	
	  Y operator+( const Y& y, int i )
	  {
	    // ... uses another operator+() in the implementation...
	    return result;
	  }
	
	  int f( const deque<int>& d )
	  {
	    // ...
	  }
	}
	//һ����Ч�Ķ��ڷ���
	//��ֲ����1:��ÿ��ͷ�ļ��У�Ϊ��������Ҫ֮�����std::���η�
	//��ֲ����2:����һ��myproject_last.h����ͷ�ļ���ʹ֮����using 
	//          namespace stdָ�Ȼ����ÿһ��ʵ���ļ��У�����������#include
	//          ָ��֮�����(#include)myproject_last.h
  //ʹ�����沽���
  
  // Example 40-3(d): Good short-term solution,
	// applying our two-step migration
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines Y
	#include <deque>
	#include <iosfwd>
	
	std::ostream& operator<<( std::ostream&, const Y& );
	  Y operator+( const Y& y, int i );
	int f( const std::deque<int>& );
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines Z
	#include <ostream>
	#include "myproject_last.h"
	                // AFTER all other #includes
	ostream& operator<<( ostream& o, const Y& y )
	{
	  // ... uses Z in the implementation ...
	  return o;
	}
	
	Y operator+( const Y& y, int i )
	{
	  // ... uses another operator+() in the implementation...
	  return result;
	}
	
	int f( const deque<int>& d )
	{
	  // ...
	}
	
	//--- common file myproject_last.h ---
	//
	using namespace std;
	//�ⲻ���ۼ����ڷ�������Ϊ�����κ����鶼����Ҫ���ڷ���ȥ����
	//ͬʱ���������ĳ��ڷ�����ȣ������򵥣���Ҫ�Ĵ����޸������١�ʵ����
	//���ô�����֧�����ֿռ�ı������Ϲ��������Ҳ������Ժ󻹵û�ͷȥ����
	//�����Ĺ��������ַ�����Ҫ�Ĺ���������
	
	//��󣬺ܿ����Ժ�ĳ���ʵ���ʱ������ʱ����Ϊ�����������ȣ���Ϳ���ʵʩ�򵥵���ֲ����
	//���ɵ�40-3(b)�����ܵĳ��ڲ��ԡ�����ѭ���²���
	
	//1.��myproject_last.h�У�ע�͵�usingָ��
	//2.���±��빤�̣�������Щ�ط�����ͨ�����룬Ȼ����ÿ��ʵ���ļ���
	//  ������ȷ��usingָ���using����
	//3.��һ������Բ�������ÿһ��ͷ�ļ���ʵ���ļ��У�������Cͷ�ļ�
	//  �����޸�Ϊ�µ�<cheader>��״
		
							
