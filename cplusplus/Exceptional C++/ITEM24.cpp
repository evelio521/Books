ITEM24:滥用/使用继承
	//元素继承的耦合性是最强的，设计时耦合性越弱越好
	
	//一下template提供了list的管理功能，包括特定的list位置上的处理元素的能力
	// Example 1 
	//
	template <class T>
	class MyList
	{
	public:
	  bool   Insert( const T&, size_t index );
	  T      Access( size_t index ) const;
	  size_t Size() const;
	private:
	  T*     buf_;
	  size_t bufsize_;
	};
	//考虑以下代码，其中呈现以MyList为基础，实作出一个MySet class 的不同做法，所有的重要元素以呈现
	// Example 1(a) 
		//
		template <class T>
		class MySet1 : private MyList<T>
		{
		public:
		  bool   Add( const T& ); // calls Insert()
		  T      Get( size_t index ) const;
		                          // calls Access()
		  using MyList<T>::Size;
		  //...
		};
		
		// Example 1(b)
		//
		template <class T>
		class MySet2
		{
		public:
		  bool   Add( const T& ); // calls impl_.Insert()
		  T      Get( size_t index ) const;
		                          // calls impl_.Access()
		  size_t Size() const;    // calls impl_.Size();
		  //...
		private:
		  MyList<T> impl_;
		};
		
    //MySet1 MySet2不具有实际意义的差别 完成的功能一样
    
    //设计准则：尽量的采用aggregation(又名composition，layering,HAS-A,delegation)来取代
              //inheritance。当我们准备塑膜出IS-IMPLEMENTED-IN-TERMS-OF关系时，采用aggregation
              //而不要采用inheritance
    //有些事情inheritance做到而containment做不到
      1.当我们需要改写虚拟式时。
      2.当我们需要处理protected members--一般而言是protected member functions，有时候是protected ctors
      3.当我们需要在一个base subobject之前先构建used object，或是再一个base subobject之后摧毁used object。我们必须用inheritance
      4.当我们需要(1)分享某个共同的virtual base classes或(2)改写某个virtual base classes的建构程序时，我们必须用inheritance
      5.当我们从empty base class的最佳化获得实质利益时。有时候，据以实作的那个class并没有任何的数据成员，也就是说他只是函数的组合。
        这种情况下以inheritance取代containment，可因empty base class最佳化之故而获得空间优势。
        简单的说，编译器允许一个empty base subobject占用零空间：虽然一个empty member object必须占用非零空间，即使不含有任何资料
        
        class B { /* ... functions only, no data ... */ }; 
				// Containment: incurs some space overhead
				//
				class D
				{
				  B b_; // b_ must occupy at least one byte,
				};      // even though B is an empty class
				// Inheritance: can incur zero space overhead
				//
				class D : private B
				{      // the B base subobject need not
				};      // occupy any space at all
		6.public inheritance总是应该塑膜IS-A的关系。nonpublic inheritance可以表现一个收到束缚的IS-A关系。
		
		
		//假设有个class DErived：public Base，从外部看，DErived物件并不是一个Base，所以他无法以多型的方式当作一个Base，因为private inheritance带来存取上的束缚，
		//然而在Derived自己 的members functions及friends内，一个DErived object可以以多型的方式被拿来当作一个base，这是应为members functions 和friend有足够的存取权限
		//如果不采用private inheritance而改用protected inheritance，那么IS-A的关系对更深的DErived classes会更明显些，意味着subclasses可以使用多型。
		
		
		//分析一下MyList MySet1 MySet2
		
		1.MyList没有protected members 。所以我们不需要继承来存取他们
		2.MyList没有虚拟函数式，不需要继承来改写他们
		3.MySet没有其他潜在的base classes，所以MyList物件不需要在另外一个basesubject之前构建或之后摧毁他。
		4.MyList没有任何的virtual base classes 啊hiMySet可能需要共享的，或其construction可能需要改写的。
		5.MyList不是空的，所以empty base classes 动机不适用
		6.MySet不是一个MyList，甚至在MySet的members functions和friend内也不是。
		
		//综述:MySet不应该继承MyList，在containment同样有效率的情况下使用inheritance，只会导入无偿的偶尔及非必要的相依性
		
		
		//有时候inheritance还是必要的
		// Example 2: Sometimes you need to inherit 
		//
		class Base
		{
		public:
		  virtual int Func1();
		protected:
		  bool Func2();
		private:
		  bool Func3(); // uses Func1
		};
		//我们需要改写Func2,或存取一个protected member Func2,就必须采用inheritance
		
		//下面的代码不是很好，不建议采用
		// Example 2(a) 
		//
		class Derived : private Base // necessary?
		{
		public:
		  int Func1();
		  // ... more functions, some of which use
		  //     Base::Func2(), some of which don't ...
		};
		//对上面的代码的改进
		// Example 2(b) 
		//
		class DerivedImpl : private Base
		{
		public:
		  int Func1();
		  // ... functions that use Func2 ...
		};
		class Derived
		{
		  // ... functions that don't use Func2 ...
		private:
		  DerivedImpl impl_;
		};
		
		//以上代码良好的隔离并封装对Base的依存性。DErived只依存Base的public介面以及DerivedImpl的public介面。
		
		
		//Containment的优点
		   1.他允许我们拥有多个used classes实体，这对inheritance而言并不实用，甚至不可能
		   2.他令used classes成为一个data members，这带来更多弹性。
		
		//对MySet2 的重写
		// Example 1(c): Generic containment 
		//
		template <class T, class Impl = MyList<T> >
		class MySet3
		{
		public:
		  bool   Add( const T& ); // calls impl_.Insert()
		  T      Get( size_t index ) const;
		                          // calls impl_.Access()
		  size_t Size() const;    // calls impl_.Size();
		  // ...
		private:
		  Impl impl_;
		};
		
		//遵循一下规则，避免常犯的错误
		1.如果nonpublic inheritance可用，绝对不要考虑public inheritance
		2.如果class之间的相互关系可以用多种方式体现，请使用其中最弱的一种关系
		
		//设计准则:总是确定public inheritance用以塑模出IS-A和WORKS-LIKE-A的关系，并符合Liskov Substitution Principle.
		          //所有被改写的member functions都必须不要求更多，也不承诺更少
		
		//设计准则:绝对不要为了重复使用base class 的代码使用public inheritance。public inheritance的目的是为了被既有的代码以多型方式重复运用base objects.
		
		//结论：
		   1.明智的运用inheritance
		   2.如果你能够单纯的以containment/delegation表现某个class的间关系，你应该那么做。
		   3.如果你需要inheritance但不想塑模出IS-A的关系，请使用nonpublic inheritance
		
				
						
				        
				      
				
						
				        
				      