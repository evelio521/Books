ITEM5:typename
	//1.什么事typename 有什么用处
	
	//如果一个名称被使用在模版声明或定义中并且依赖于模版参数，
	//则这个名称不被认为是一个类型的名称，除非名称查找到了一个
	//合适的类型名称，或这个名称使用typename修饰
	
	//2.下列代码有错吗
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
	//编译器不能解释出instantiated_type()
	//依赖性的名称只有在实例化时候可见
	//应该向下面一样写
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
		//或这样
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
		
		
		//一个玩笑
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
		
		
							
							
								