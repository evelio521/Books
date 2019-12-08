ITEM5:typename
	//1.ʲô��typename ��ʲô�ô�
	
	//���һ�����Ʊ�ʹ����ģ�����������в���������ģ�������
	//��������Ʋ�����Ϊ��һ�����͵����ƣ��������Ʋ��ҵ���һ��
	//���ʵ��������ƣ����������ʹ��typename����
	
	//2.���д����д���
	template<typename T>
	class X_base
	{
	public:
	  typedef T instantiated_type;
	};
	
	template<typename A, typename B>
	class X : public X_base<B>
	{
	public:
	  bool operator()( const instantiated_type& i ) const
	  {
	    return i != instantiated_type();
	  }
	
	  // ... more stuff ...
	};
	//���������ܽ��ͳ�instantiated_type()
	//�����Ե�����ֻ����ʵ����ʱ��ɼ�
	//Ӧ��������һ��д
	// Example 5-2(a): Somewhat horrid
		//
		template<typename A, typename B>
		class X : public X_base<B>
		{
		public:
		  bool operator()(
		    const typename X_base<B>::instantiated_type& i
		    ) const
		  {
		    return i != typename X_base<B>::instantiated_type();
		  }
		
		  // ... more stuff ...
		};
		//������
		// Example 5-2(b): Better
		//
		template<typename A, typename B>
		class X : public X_base<B>
		{
		public:
		  typedef typename X_base<B>::instantiated_type
		          instantiated_type;
		
		  bool operator()( const instantiated_type& i ) const
		  {
		    return i != instantiated_type();
		  }
		
		  // ... more stuff ...
		};
		
		
		//һ����Ц
		#include <iostream>

		class Rose {};
		
		class A { public: typedef Rose rose; };
		
		template<typename T>
		class B : public T { public: typedef typename T::rose foo; };
		
		template<typename T>
		void smell( T ) { std::cout << "awful"; }
		
		void smell( Rose ) { std::cout << "sweet"; }
		
		int main()
		{
		  smell( A::rose() );
		  smell( B<A>::foo() );  // :-)
		}
		
		//
		#include <iostream>

		class Rose {};
		
		class A { public: typedef Rose rose; };
		
		template<typename T>
		class B : public T { public: typedef  T foo; };
		
		template<typename T>
		void smell( T ) { std::cout << "awful"; }
		
		void smell( Rose ) { std::cout << "sweet"; }
		
		int main()
		{
		  smell( A::rose() );
		  smell( B<A>::foo() );  // :-)
		}
		
		
							
							
								