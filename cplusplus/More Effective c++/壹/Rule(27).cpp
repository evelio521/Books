 规则27:
 	/*：要求或禁止在堆中产生对象*/
 	/*
 	 * 要求在堆中建立对象
   * 让我们先从必须在堆中建立对象开始说起。为了执行这种限制，
   * 你必须找到一种方法禁止以调用“new”以外的其它手段建立对象。
   * 这很容易做到。非堆对象（non-heap object）在定义它的地方被
   * 自动构造，在生存时间结束时自动被释放，所以只要禁止使用隐式
   * 的构造函数和析构函数，就可以实现这种限制。
   */
   class UPNumber {
			public:
				UPNumber();
				UPNumber(int initValue);
				UPNumber(double initValue);
				UPNumber(const UPNumber& rhs);
					// 伪析构函数 (一个const 成员函数， 因为
					// 即使是const对象也能被释放。)
				void destroy() const { delete this; }
				...
			private:
				~UPNumber();
	};
		//然后客户端这样进行程序设计：
		UPNumber n; // 错误! (在这里合法， 但是
								// 当它的析构函数被隐式地
								// 调用时，就不合法了)
		UPNumber *p = new UPNumber; //正确
		...
		delete p; // 错误! 试图调用
		// private 析构函数
		p->destroy(); // 正确
		
		/*
		 * 另一种方法是把全部的构造函数都声明为private。这种方法的缺点
		 * 是一个类经常有许多构造函数，类的作者必须记住把它们都声明为
		 * private。否则如果这些函数就会由编译器生成，构造函数包括拷贝
		 * 构造函数，也包括缺省构造函数；编译器生成的函数总是public
		 *（参见Effecitve C++ 条款45）。因此仅仅声明析构函数为private
		 * 是很简单的，因为每个类只有一个析构函数。
		 */
		 
		 
		 
		 
		 
		 /*
		  * 通过限制访问一个类的析构函数或它的构造函数来阻止建立非堆对象，
		  * 但是在条款26已经说过，这种方法也禁止了继承和包容（containment）
		  * 这些困难不是不能克服的。通过把UPNumber的析构函数声明为protected
		  *（同时它的构造函数还保持public）就可以解决继承的问题，需要包含UPNumber
		  * 对象的类可以修改为包含指向UPNumber的指针
		  */
		  class UPNumber { ... }; // 声明析构函数为protected
			class NonNegativeUPNumber:
				public UPNumber { ... }; // 现在正确了; 派生类 能够访问 protected 成员
			class Asset {
				public:
					Asset(int initValue);
					~Asset();
					...
				private:
					UPNumber *value;
		};
		Asset::Asset(int initValue): value(new UPNumber(initValue)) // 正确
			{ ... }
		Asset::~Asset()
			{ value->destroy(); } // 也正确
			
			/*判断一个对象是否在堆中*/
			class HeapTracked { // 混合类; 跟踪
				public: // 从operator new返回的ptr
					class MissingAddress{}; // 异常类，见下面代码
					virtual ~HeapTracked() = 0;
					static void *operator new(size_t size);
					static void operator delete(void *ptr);
					bool isOnHeap() const;
					private:
					typedef const void* RawAddress;
					static list<RawAddress> addresses;
		};

		// mandatory definition of static class member
		list<RawAddress> HeapTracked::addresses;
		// HeapTracked的析构函数是纯虚函数，使得该类变为抽象类。
		// (参见Effective C++条款14). 然而析构函数必须被定义，
		//所以我们做了一个空定义。.
		HeapTracked::~HeapTracked() {}
		void * HeapTracked::operator new(size_t size)
		{
			void *memPtr = ::operator new(size); // 获得内存
			addresses.push_front(memPtr); // 把地址放到list的前端
			return memPtr;
		}
		void HeapTracked::operator delete(void *ptr)
		{
			//得到一个 "iterator"，用来识别list元素包含的ptr；
			//有关细节参见条款35
			list<RawAddress>::iterator it =
			find(addresses.begin(), addresses.end(), ptr);
			if (it != addresses.end()) { // 如果发现一个元素
				addresses.erase(it); //则删除该元素
				::operator delete(ptr); // 释放内存
			} else { // 否则
					throw MissingAddress(); // ptr就不是用operator new
			} // 分配的，所以抛出一个异常
			}
		bool HeapTracked::isOnHeap() const
		{
			// 得到一个指针，指向*this占据的内存空间的起始处，
			// 有关细节参见下面的讨论
			const void *rawAddress = dynamic_cast<const void*>(this);
			// 在operator new返回的地址list中查到指针
			list<RawAddress>::iterator it =
			find(addresses.begin(), addresses.end(), rawAddress);
			return it != addresses.end(); // 返回it是否被找到
		}
		
		//使用这个类，即使是最初级的程序员也可以在类中加入跟踪堆中指针的功能。
		//他们所需要做的就是让他们的类从HeapTracked继承下来。例如我们想判断
		//Assert对象指针指向的是否是堆对象：
		class Asset: public HeapTracked {
				private:
				UPNumber value;
				...
		};
		//我们能够这样查询Assert*指针，如下所示：
			void inventoryAsset(const Asset *ap)
				{
					if (ap->isOnHeap()) {
				ap is a heap-based asset — inventory it as such;
					}
					else {
				ap is a non-heap-based asset — record it that way;
					}
				}
		//象HeapTracked这样的混合类有一个缺点，它不能用于内建类型，
		//因为象int和char这样的类型不能继承自其它类型。不过使用象
		//HeapTracked的原因一般都是要判断是否可以调用“delete this”，
		//你不可能在内建类型上调用它，因为内建类型没有this指针。
		
		/*禁止堆对象*/
		
		//禁止用户直接实例化对象很简单，因为总是调用new来建立这种对象，
		//你能够禁止用户调用new。你不能影响new操作符的可用性（这是内嵌于语言的），
		//但是你能够利用new操作符总是调用operator new函数这点（参见条款M8），
		//来达到目的。你可以自己声明这个函数，而且你可以把它声明为private。
		//例如，如果你想不想让用户在堆中建立UPNumber对象，你可以这样编写：
		class UPNumber {
				private:
					static void *operator new(size_t size);
					static void operator delete(void *ptr);
...
};
		//现在用户仅仅可以做允许它们做的事情：
		UPNumber n1; // okay
		static UPNumber n2; // also okay
		UPNumber *p = new UPNumber; // error! attempt to call
		// private operator new
		//把operator new声明为private就足够了，但是把operator new声明为private，
		//而把iperator delete声明为public，这样做有些怪异，所以除非有绝对需要的原因，
		//否则不要把它们分开声明，最好在类的一个部分里声明它们。如果你也想禁止
		//UPNumber堆对象数组，可以把operator new[]和operator delete[]（参见条款M8）也声明为private。
		
		
		//把operator new声明为private经常会阻碍UPNumber对象做为一个位于堆
		//中的派生类对象的基类被实例化。因为operator new和operator delete是
		//自动继承的，如果operator new和operator delete没有在派生类中被声明
		//为public（进行改写，overwrite），它们就会继承基类中private的版本，如下所示：
		class UPNumber { ... }; // 同上
		class NonNegativeUPNumber: //假设这个类
			public UPNumber { //没有声明operator new
			...
		};
		NonNegativeUPNumber n1; // 正确
		static NonNegativeUPNumber n2; // 也正确
		NonNegativeUPNumber *p =new NonNegativeUPNumber; // 错误! 试图调用
 																					// private operator new
 																					
 		//UPNumber的operator new是private这一点，不会对包含UPNumber成员对象的对象的分配产生任何影响：
			class Asset {
				public:
					Asset(int initValue);
					...
				private:
					UPNumber value;
		};
		Asset *pa = new Asset(100); // 正确, 调用
		// Asset::operator new 或
		// ::operator new, 不是
		// UPNumber::operator new