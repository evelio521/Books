ITEM29:ʹ��auto_ptr
	//1.ʹ��auto_ptr�ĳ�����������������ʲô���⣿
	template<typename T>
	void f( size_t n ) {
	  auto_ptr<T> p1( new T );
	  auto_ptr<T> p2( new T[n] );
	
	  // ... more processing ...
	}
	//p2���������� auot_ptrֻ���������������������Զ����Լ�ӵ�е�ָ�롣auto_ptr����
	//�����delete������delete[] ������ʹ����ͨ��delete��p1�ᱻ��ȷ�������p2�򲻻ᡣ
	
	//���ʹ�ô����delete��������ʵ�ʺ��ȡ������ı���������ý�����ڴ�й©��������
	//�����ڴ汻�ƻ���Ȼ��������������Ĵ������������һ��Ч����������
	#include <iostream>
	#include <memory>
	#include <string>
	using namespace std;
	int c = 0;
	
	class X {
	public:
	  X() : s( "1234567890" ) { ++c; }
	 ~X()                     { --c; }
	    string s;
	};
	
	template<typename T>
	void f( size_t n )
	{
	  {
	    auto_ptr<T> p1( new T );
	    auto_ptr<T> p2( new T[n] );
	  }
	  cout << c << " ";   // report # of X objects
	}                     // that currently exist
	
	int main()
	{
	  while( true )
	  {
	    f<X>(100);
	  }
	}
	
	//�㳤�ȵ���������ȫ�Ϸ��� new T[0] ��new T[n]��һ������Ϊ����
	//auto_ptr����ӵ���㳤������
	
	
	//2.��ν����һ���⣿�������ṩ����ķ�������������Adapterģʽ���滻
	//������ṹ���滻auto_ptr��
	
	//����(1) �����Լ���auto_ptr
	//       ����(1)(a)  ͨ����auto_ptr����
	          /*
	           * �ŵ㼸��û��
	           * ȱ��ٲ�ʤ��
	           */
	//       ����(1)(b)  ͨ������auto_ptr�Ĵ���
	          /*
	           * ȥ��������е�auot_ptr���롣�����е�delete��Ϊdelete[]
	           * �е�: ����ʵ�֡�û�пռ��ʱ���ϵ���������
	           * ȱ��: ����ά��
	           */
	           
	           
	//����2:����Adapterģʽ
	  
	    //���������˼· ���ǲ���ôд
	    auto_ptr<T> p2( new T[n] );
			//������ôд
			auto_ptr< ArrDelAdapter<T> >
              p2( new ArrDelAdapter<T>(new T[n]) );
      //���У�ArrDelAdapter����һ������ΪTָ��Ĺ��캯����
      //�����������У�����delete[]
      template<typename T>
			class ArrDelAdapter {
			public:
			  ArrDelAdapter( T* p ) : p_(p) { }
			 ~ArrDelAdapter() { delete[] p_; }
			  // operators like "->" "T*" and other helpers
			private:
			  T* p_;
			};
			//��Ȼֻ��һ��ArrDelAdapter<T>����~auto_ptr()�еĵ���������ʽ��delete�Ͳ���������
			//��Ϊ~ArrDelAdapter<T>��������ȷ����delete[]
			
			//�е�:����ʵ��
			//ȱ��:�ɶ����Բ����ʹ�ã������ռ��ϵĿ���

      //����3: ���ֹ���д���쳣�����߼�ȡ��auto_ptr
			//�����ֹ���Ϊp2�������auto_ptr������£��ֹ���д�Լ����쳣�����߼�
			//���ǲ���ôд
			auto_ptr<T> p2( new T[n] );
			//
			// ... more processing ...
			//
			//������ôд
			T* p2( new T[n] );
			try {
			  //
			  // ... more processing
			  //
			}
			delete[] p2;
			//�е�:����ʹ�ã�û�пռ��ϵĿ���
			//ȱ��:����ʵ�֣�ȱ����׳�ԣ��ɶ��Բ�
			
			//����4:��vector�������
			//���ǲ���ôд
			auto_ptr<T> p2( new T[n] );
			//������ôд
			vector<T> p2( n );
			//�е�: ����ʵ�֣��ɶ��Ժã�����׳����ߣ�û�пռ��ʱ���ϵĿ���
			//ȱ��: �﷨�ı䣬ʹ���Ըı�
			
			//���׼��:����ʹ��vector����Ҫʹ���ڽ���ָ��





			

	
	
