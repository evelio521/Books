规则28:
	/*智能指针*/
	
	//大部分智能指针模版的样子
	class SmartPtr {
		public:
			SmartPtr(T* realPtr = 0); // 建立一个灵巧指针
				// 指向dumb pointer所指的
				// 对象。未初始化的指针
				// 缺省值为0(null)
			SmartPtr(const SmartPtr& rhs); // 拷贝一个灵巧指针
			~SmartPtr(); // 释放灵巧指针
			// make an assignment to a smart ptr
			SmartPtr& operator=(const SmartPtr& rhs);
			T* operator->() const; // dereference一个灵巧指针
			// 以访问所指对象的成员
			T& operator*() const; // dereference 灵巧指针
		private:
			T *pointee; // 灵巧指针所指的对象
};

  
  //智能指针的构造、赋值和析构
  
  template<class T>
		class auto_ptr {
			public:
				...
				auto_ptr(auto_ptr<T>& rhs); // 拷贝构造函数
				auto_ptr<T>& // 赋值
				operator=(auto_ptr<T>& rhs); // 操作符
				...
		};
	template<class T>
	auto_ptr<T>::auto_ptr(auto_ptr<T>& rhs)
	{
		pointee = rhs.pointee; // 把*pointee的所有权
			// 传递到 *this
		rhs.pointee = 0; // rhs不再拥有
	} // 任何东西
	template<class T>
	auto_ptr<T>& auto_ptr<T>::operator=(auto_ptr<T>& rhs)
	{
		if (this == &rhs) // 如果这个对象自我赋值
			return *this; // 什么也不要做
		delete pointee; // 删除现在拥有的对象
		pointee = rhs.pointee; // 把*pointee的所有权
		rhs.pointee = 0; // 从 rhs 传递到 *this
		return *this;
}

	//因为当调用auto_ptr的拷贝构造函数时，对象的所有权被传递出去，
	//所以通过传值方式传递auto_ptr对象是一个很糟糕的方法。因为：
  // 这个函数通常会导致灾难发生
		void printTreeNode(ostream& s, auto_ptr<TreeNode> p)
			{ s << *p; }
		int main()
		{
			auto_ptr<TreeNode> ptn(new TreeNode);
			...
			printTreeNode(cout, ptn); //通过传值方式传递auto_ptr
			...
		}
		
		//这不是说你不能把auto_ptr做为参数传递，这只意味着不能使用传值的方法。
		//通过const引用传递（Pass-by-reference-to-const）的方法是这样的：
		// 这个函数的行为更直观一些
		void printTreeNode(ostream& s,const auto_ptr<TreeNode>& p)
			{ s << *p; }
	//智能指针的析构函数通常是这样的：
		template<class T>
		SmartPtr<T>::~SmartPtr()
		{
			if (*this owns *pointee) {
			delete pointee;
			}
		}
		
		/*实现解引用操作符*/
		template<class T>
		T& SmartPtr<T>::operator*() const
		{
			perform "smart pointer" processing;
			return *pointee;
		}
		
		template<class T>
		T* SmartPtr<T>::operator->() const
		{
			perform "smart pointer" processing;
			return pointee;
		}
		
		/*测试智能指针是否为NULL*/
		//目前为止我们讨论的函数能让我们建立、释放、拷贝、赋值、dereference灵巧指针。
		//但是有一件我们做不到的事情是“发现灵巧指针为NULL”：
		SmartPtr<TreeNode> ptn;
		...
		if (ptn == 0) ... // error!
		if (ptn) ... // error!
		if (!ptn) ... // error!
		//这是一个严重的限制。
		
		//如果这样写下面的代码来解决问题 会带来一定的问题
		template<class T>
		class SmartPtr {
			public:
				...
				operator void*(); // 如果灵巧指针为null，
				... // 返回0， 否则返回
		}; // 非0。
		SmartPtr<TreeNode> ptn;
		...
		if (ptn == 0) ... // 现在正确
		if (ptn) ... // 也正确
		if (!ptn) ... // 正确
	//这与iostream类中提供的类型转换相同，所以可以这样编写代码：
		ifstream inputFile("datafile.dat");
		if (inputFile) ... // 测试inputFile是否已经被
		// 成功地打开。
		//象所有的类型转换函数一样，它有一个缺点：在一些情况下虽然大
		//多数程序员希望它调用失败，但是函数确实能够成功地被调用（参见条款M5）。
		//特别是它允许灵巧指针与完全不同的类型之间进行比较：
			SmartPtr<Apple> pa;
			SmartPtr<Orange> po;
			...
			if (pa == po) ... // 这能够被成功编译!
				
		//一种差强人意的解决办法
		template<class T>
		class SmartPtr {
			public:
				...
				bool operator!() const; // 当且仅当灵巧指针是
				... // 空值，返回true。
		};
		//用户程序如下所示：
		SmartPtr<TreeNode> ptn;
			...
		if (!ptn) { // 正确
		... // ptn 是空值
		}
		else {
		... // ptn不是空值
		}
		//但是这样就不正确了：
		if (ptn == 0) ... // 仍然错误
		if (ptn) ... // 也是错误的
		//仅在这种情况下会存在不同类型之间进行比较：
		SmartPtr<Apple> pa;
		SmartPtr<Orange> po;
		...
		if (!pa == !po) ... // 能够编译
			
		/*把智能指针转变成dumb指针*/
		class Tuple { ... }; // 同上
		void normalize(Tuple *pt); // 把*pt 放入
		// 范式中; 注意使用的
		// 是dumb指针
		//考虑一下，如果你试图用指向Tuple的灵巧指针作参数调用normalize，会出现什么情况：
		DBPtr<Tuple> pt;
		...
		normalize(pt); // 错误!
		//除非有一个让人非常信服的原因去这样做 否则不要提供对dumb pointer的隐式类型转换操作符。
		
		/*smart pointer和 "与继承有关的"类型转换*/
		
		class MusicProduct { 
			public: 
				MusicProduct(const string& title); 
				virtual void play() const = 0; 
				virtual void displayTitle() const = 0;
				... 
		};
		 class Cassette: public MusicProduct { 
		 	public: 
		 		Cassette(const string& title); 
		 		virtual void play() const; 
		 		virtual void displayTitle() const; 
		 		... 
		 };
		 class CD: public MusicProduct { 
		 	public: 
		 		CD(const string& title); 
		 		virtual void play() const; 
		 		virtual void displayTitle() const;
		 		 ... 
		 };
   //再接着假设，我们有一个函数，给它一个MusicProduct对象，
   //它能显示产品名，并播放它： 
   void displayAndPlay(const MusicProduct* pmp, int numTimes) 
   { 
   		for (int i = 1; i <= numTimes; ++i) 
   			{ 
   				pmp->displayTitle(); 
   				pmp->play();
   			 } 
   } 
   //这个函数能够这样使用： 
   Cassette *funMusic = new Cassette("Alapalooza"); 
   CD *nightmareMusic = new CD("Disco Hits of the 70s"); 
   displayAndPlay(funMusic, 10); 
   displayAndPlay(nightmareMusic, 0); 
   //这并没有什么值得惊讶的东西，但是当我们用灵巧指针替代dumb指针，会发生什么呢：
	void displayAndPlay(const SmartPtr<MusicProduct>& pmp, int numTimes); 
	SmartPtr<Cassette> funMusic(new Cassette("Alapalooza")); 
	SmartPtr<CD> nightmareMusic(new CD("Disco Hits of the 70s")); 
	displayAndPlay(funMusic, 10); // 错误! 
	displayAndPlay(nightmareMusic, 0); // 错误! 
	
	//既然灵巧指针这么聪明，为什么不能编译这些代码呢？
	// 不能进行编译的原因是不能把SmartPtr<CD>或SmartPtr<Cassette>
	//转换成SmartPtr<MusicProduct>。从编译器的观点来看，这些类之间
	//没有任何关系。为什么编译器的会这样认为呢？毕竟SmartPtr<CD> 或
	// SmartPtr<Cassette>不是从SmartPtr<MusicProduct>继承过来的，
	//这些类之间没有继承关系，我们不可能要求编译器把一种对象转换成
	//（完全不同的）另一种类型的对象。
	
	//解决办法是
	template<class T> // 模板类，指向T的 
	class SmartPtr { // 灵巧指针 
		public: 
			SmartPtr(T* realPtr = 0); 
			T* operator->() const; T& operator*() const; 
			template<class newType> // 模板成员函数 
			operator SmartPtr<newType>() // 为了实现隐式类型转换. 
			{ 
				return SmartPtr<newType>(pointee); 
				} 
			... 
	};
	void displayAndPlay(const SmartPtr<MusicProduct>& pmp, int numTimes); 
	SmartPtr<Cassette> funMusic(new Cassette("Alapalooza")); 
	SmartPtr<CD> nightmareMusic(new CD("Disco Hits of the 70s")); 
	displayAndPlay(funMusic, 10); // 正确! 
	displayAndPlay(nightmareMusic, 0); // 正确!
	
	//下面代码调用的过程
	displayAndPlay(funMusic, 10); 
	
	/* 
	 *funMusic对象的类型是SmartPtr<Cassette>。函数displayAndPlay期望的参数是
	 * SmartPtr<MusicProduct>地对象。编译器侦测到类型不匹配，于是寻找把funMusic
	 * 转换成SmartPtr<MusicProduct>对象的方法。它在SmartPtr<MusicProduct>类里寻
	 * 找带有SmartPtr<Cassette>类型参数的单参数构造函数（参见条款M5），但是没有找到。
	 * 然后它们又寻找成员函数模板，以实例化产生这样的函数。它们在SmartPtr<Cassette>
	 * 发现了模板，把newType绑定到MusicProduct上，生成了所需的函数
	 */
	 
	 //这种技术能给我们几乎所有想要的行为特性。假设我们用一个新类CasSingle来扩充
	 //MusicProduct类层次，用来表示cassette singles。
	 template<class T> // 同上, 包括作为类型 
	 class SmartPtr { ... }; // 转换操作符的成员模板 
	 void displayAndPlay(const SmartPtr<MusicProduct>& pmp, int howMany); 
	 void displayAndPlay(const SmartPtr<Cassette>& pc, int howMany); 
	 SmartPtr<CasSingle> dumbMusic(new CasSingle("Achy Breaky Heart")); 
	 displayAndPlay(dumbMusic, 1); // 错误! 
	 //在这个例子里，displayAndPlay被重载，一个函数带有SmartPtr<Cassette> 对象参数，
	 //其它函数的参数为SmartPtr<CasSingle>，我们期望调用SmartPtr<Cassette>，
	 //因为CasSingle是直接从Cassette上继承下来的，而间接继承自MusicProduct。
	 //当然这是dumb指针时的工作方法。我们的灵巧指针不会这么灵巧，它们的转换操
	 //作符是成员函数，对C++编译器而言，所有类型转换操作符是同等地位的。
	 //因此displayAndPlay的调用具有二义性，因为从SmartPtr<CasSingle> 到
	 //SmartPtr<Cassette>的类型转换并不比到SmartPtr<MusicProduct>的类型转换优先。
	 
	 /*smart pointer与const*/
	 
	 CD goodCD("Flood");
		const CD *p;// p 是一个non-const 指针
							//指向 const CD 对象
		CD * const p = &goodCD; // p 是一个const 指针
									// 指向non-const CD 对象
									// 因为 p 是const, 它
									// 必须被初始化
		const CD * const p = &goodCD; // p 是一个const
											// 指向一个 const CD 对
		//我们自然想要让灵巧指针具有同样的灵活性。不幸的是只能在一个地方放置const，
		//并只能对指针本身起作用，而不能针对于所指对象：
		 const SmartPtr<CD> p = &goodCD;// p 是一个const 灵巧指针  
		 																// 指向 non-const CD 对象
		 																
		//考虑下面四种情况
		SmartPtr<CD> p; // non-const 对象 // non-const 指针 
		SmartPtr<const CD> p; // const 对象, // non-const 指针 
		const SmartPtr<CD> p = &goodCD; // non-const 对象 // const指针 
		const SmartPtr<const CD> p = &goodCD; // const 对象 // const 指针
		
		CD *pCD = new CD("Famous Movie Themes"); 
		const CD * pConstCD = pCD; // 正确 
		//但是如果我们试图把这种方法用在灵巧指针上，情况会怎么样呢？ 
		SmartPtr<CD> pCD = new CD("Famous Movie Themes"); 
		SmartPtr<const CD> pConstCD = pCD; // 正确么?不 SmartPtr<CD> 与SmartPtr<const CD>是完全不同的类型
		
		//上述问题的解决方法
		template<class T> // 指向const对象的 
		class SmartPtrToConst { // 灵巧指针 
			... // 灵巧指针通常的 
					// 成员函数 
			protected: 
				union { const T* constPointee; // 让 SmartPtrToConst 访问 
					T* pointee; // 让 SmartPtr 访问 
					}; 
			};
			 template<class T> // 指向non-const对象 
			 class SmartPtr: // 的灵巧指针 
			 	public SmartPtrToConst<T> {
			 		 ... // 没有数据成员
			};
			
			//利用这种新设计，我们能够获得所要的行为特性： 
			SmartPtr<CD> pCD = new CD("Famous Movie Themes"); 
			SmartPtrToConst<CD> pConstCD = pCD; // 正确
	