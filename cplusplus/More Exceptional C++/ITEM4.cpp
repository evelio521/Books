ITEM4:使用继承还是traits
	//1.什么是traits类
	
	 //in the standard itself std::char_traits<T> gives information about 
	 //the character-like type T, particularly how to compare and manipulate
	 // such T objects. This information is used in such templates as std::basic_string
	 // and std:: basic_ostream to allow them to work with character types that are
	 // not necessarily char or wchar_t, including working with user-defined types 
	 //for which you provide a suitable specialization of std::char_traits. Similarly,
	 // std::iterator_traits provides information about iterators that other templates,
	 // particularly algorithms and containers, can put to good use. Even std::numeric_limits 
	 //gets into the traits act, providing information about the capabilities and behavior of
	 // various kinds of numeric types as they're implemented on your particular platform and compiler.
	//2.示范如何检测好使用模版类成员。
		
		//You want to write a class template C that can be instantiated only on types that 
		//have a member function named Clone() that takes no parameters and returns a pointer
		// to the same kind of object.


		// Example 4-2
		//
		// T must provide T* T::Clone() const
		template<typename T>
		class C
		{
		  // ...
		};
		
		//方法1
		// Example 4-2(a): Initial attempt,
		// sort of requires Clone()
		//
		// T must provide /*...*/ T::Clone( /*...*/ )
		template<typename T>
		class C
		{
		public:
		  void SomeFunc( const T* t )
		  {
		    // ...
		    t->Clone();
		    // ...
		  }
		};
		//方法2
		// Example 4-2(b): Revised attempt, requires Clone()
		//
		// T must provide /*...*/ T::Clone( /*...*/ )
		template<typename T>
		class C
		{
		public:
		  ~C()
		  {
		    // ...
		    const T t; // kind of wasteful, plus also requires
		               // that T have a default constructor
		    t.Clone();
		    // ...
		  }
		};
		
		//方法3
		// Example 4-2(c): Better, requires
		// exactly T* T::Clone() const
		//
		// T must provide T* T::Clone() const
		template<typename T>
		class C
		{
		public:
		  // in C's destructor (easier than putting it
		  // in every C constructor):
		  ~C()
		  {
		    T* (T::*test)() const = &T::Clone;
		    test; // suppress warnings about unused variables
		    // this unused variable is likely to be optimized
		    // away entirely
		
		    // ...
		  }
		
		  // ...
		};
		//方法4
		// Example 4-2(d): Alternative way of requiring
		// exactly T* T::Clone() const
		//
		// T must provide T* T::Clone() const
		template<typename T>
		class C
		{
		  bool ValidateRequirements() const
		  {
		    T* (T::*test)() const = &T::Clone;
		    test; // suppress warnings about unused variables
		    // ...
		    return true;
		  }
		
		public:
		  // in C's destructor (easier than putting it
		  // in every C constructor):
		 ~C()
		  {
		    assert( ValidateRequirements() );
		  }
		
		  // ...
		};
		方法5
		// Example 4-2(e): Using constraint inheritance
		// to require exactly T* T::Clone() const
		//
		
		// HasClone requires that T must provide
		// T* T::Clone() const
		template<typename T>
		class HasClone
		{
		public:
		  static void Constraints()
		  {
		    T* (T::*test)() const = &T::Clone;
		    test; // suppress warnings about unused variables
		  }
		  HasClone() { void (*p)() = Constraints; }
		};
		
		template<typename T>
		class C : HasClone<T>
		{
		  // ...
		};
		//3.某个程序员想写一个模版，模版要求:他在实例化时所使用的类型
		   //具有一个clone()成员函数，这个程序员采用的方案基于这样一个要求
		   //提供clone()的类必须派生于某个拥有的Cloneable基类
		   
		  // Example 4-3(a): An IsDerivedFrom1 value helper
			//
			// Advantages: Can be used for compile-time value test
			// Drawbacks:  Pretty complex
			//
			template<typename D, typename B>
			class IsDerivedFrom1
			{
			  class No { };
			  class Yes { No no[2]; };
			
			  static Yes Test( B* ); // declared, but not defined
			  static No Test( ... ); // declared, but not defined
			
			public:
			  enum { Is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) };
			};
			//
			// Example 4-3(a), continued: Using IsDerivedFrom1
			// helper to enforce derivation from Cloneable
			//
			template<typename T>
			class X
			{
			  bool ValidateRequirements() const
			  {
			    // typedef needed because otherwise the , will be
			    // interpreted as delimiting macro parameters to assert
			    typedef IsDerivedFrom1<T, Cloneable> Y;
			
			    // a runtime check, but one that can be turned
			    // into a compile-time check without much work
			    assert( Y::Is );
			
			    return true;
			  }
			
			public:
			  // in X's destructor (easier than putting it
			  // in every X constructor):
			  ~X()
			  {
			  assert( ValidateRequirements() );
			  }
			
			  // ...
			};
			//解法2
			// Example 4-3(b): An IsDerivedFrom2 constraints base class
			//
			// Advantages: Compile-time evaluation
			//             Simpler to use directly
			// Drawbacks:  Not directly usable for compile-time value test
			//
			template<typename D, typename B>
			class IsDerivedFrom2
			{
			  static void Constraints(D* p)
			  {
			    B* pb = p;
			    pb = p; // suppress warnings about unused variables
			  }
			
			protected:
			  IsDerivedFrom2() { void(*p)(D*) = Constraints; }
			};
			// Force it to fail in the case where B is void
			template<typename D>
			class IsDerivedFrom2<D, void>
			{
			  IsDerivedFrom2() { char* p = (int*)0; /* error */ }
			};
			
			Now the check is much simpler:
			
			// Example 4-3(b), continued: Using IsDerivedFrom2
			// constraints base to enforce derivation from Cloneable
			//
			template<typename T>
			class X : IsDerivedFrom2<T,Cloneable>
			{
			  // ...
			};
			
			//上面2种解法的综合
			// Example 4-3(c): An IsDerivedFrom constraints base
			// with testable value
			//
			template<typename D, typename B>
			class IsDerivedFrom
			{
			  class No { };
			  class Yes { No no[2]; };
			
			  static Yes Test( B* ); // not defined
			  static No Test( ... ); // not defined
			  static void Constraints(D* p) { B* pb = p; pb = p; }
			
			public:
			  enum { Is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) };
			
			  IsDerivedFrom() { void(*p)(D*) = Constraints; }
			};
			
			//选择不同实现版本
			// Example 4-3(d): Using IsDerivedFrom to make use of
			// derivation from Cloneable if available, and do
			// something else otherwise.
			//
			template<typename T, int>
			class XImpl
			{
			  // general case: T is not derived from Cloneable
			};
			
			template<typename T>
			class XImpl<T, 1>
			{
			  // T is derived from Cloneable
			};
			
			template<typename T>
			class X
			{
			  XImpl<T, IsDerivedFrom<T, Cloneable>::Is> impl_;
			  // ... delegates to impl_ ...
			};
			
			//4. 对3中的解法使用traits
												
			// Example 4-4: Using traits instead of IsDerivedFrom
			// to make use of Cloneability if available, and do
			// something else otherwise. Requires writing a
			// specialization for each Cloneable class.
			//
			template<typename T>
			class XTraits
			{
			public:
			  // general case: use copy constructor
			  static T* Clone( const T* p ) { return new T( *p ); }
			};
			
			template<>
			class XTraits<MyCloneable>
			{
			public:
			  // MyCloneable is derived from Cloneable, so use Clone()
			  static MyCloneable* Clone( const MyCloneable* p )
			  {
			    return p->Clone();
			  }
			};
			
												
																						
