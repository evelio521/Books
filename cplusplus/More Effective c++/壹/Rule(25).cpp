规则25:
	/*将构造函数和非成员函数虚拟化*/
	//virtual constructor
	class NLComponent { //用于 newsletter components
		public: // 的抽象基类
			... //包含至少一个纯虚函数
		private:
			
		};
	class TextBlock: public NLComponent {
		public:
			... // 不包含纯虚函数
		};
	class Graphic: public NLComponent {
		public:
			... // 不包含纯虚函数
		};
	class NewsLetter { // 一个 newsletter 对象
		public: // 由NLComponent 对象
			... // 的链表组成
		private:
			// 为建立下一个NLComponent对象从str读取数据,
			// 建立component 并返回一个指针。
			static NLComponent * readComponent(istream& str);
			list<NLComponent*> components;
		};
		
	NewsLetter::NewsLetter(istream& str)
		{
			while (str) {
			// 把readComponent返回的指针添加到components链表的最后，
			// "push_back" 一个链表的成员函数，用来在链表最后进行插入操作。
			components.push_back(readComponent(str));
				}
		}
		
		//virtual copy constructor
		class NLComponent {
			public:
				// declaration of virtual copy constructor
				virtual NLComponent * clone() const = 0;
				...
		};
		class TextBlock: public NLComponent {
			public:
				virtual TextBlock * clone() const // virtual copy
						{ return new TextBlock(*this); } // constructor
				...
		};
		class Graphic: public NLComponent {
			public:
				virtual Graphic * clone() const // virtual copy
					{ return new Graphic(*this); } // constructor
				...
		};
		class NewsLetter {
			public:
				NewsLetter(const NewsLetter& rhs);
				...
			private:
				list<NLComponent*> components;
		};
		NewsLetter::NewsLetter(const NewsLetter& rhs)
		{
			// 遍历整个rhs链表，使用每个元素的虚拟拷贝构造函数
			// 把元素拷贝进这个对象的component链表。
			// 有关下面代码如何运行的详细情况，请参见条款M35.
			for (list<NLComponent*>::const_iterator it =
				rhs.components.begin();
				it != rhs.components.end();
				++it) {
						// "it" 指向rhs.components的当前元素，调用元素的clone函数，
						// 得到该元素的一个拷贝，并把该拷贝放到
						// 这个对象的component链表的尾端。
				components.push_back((*it)->clone());
				}
		}
		
		/*
		 * 注意上述代码的实现利用了最近才被采纳的较宽松的虚拟函数返回值类型规则。
		 * 被派生类重定义的虚拟函数不用必须与基类的虚拟函数具有一样的返回类型。
		 * 如果函数的返回类型是一个指向基类的指针（或一个引用），那么派生类的函
		 * 数可以返回一个指向基类的派生类的指针（或引用）。这不是C++的类型检查上的
		 * 漏洞，它使得有可能声明象虚拟构造函数这样的函数.
		 */
		 
		 //将non-member functions的行为虚化
		 //下面的代码会带来麻烦
		 class NLComponent {
			public:
				// 对输出操作符的不寻常的声明
				virtual ostream& operator<<(ostream& str) const = 0;
			...
		};
		class TextBlock: public NLComponent {
			public:
				// 虚拟输出操作符(同样不寻常)
				virtual ostream& operator<<(ostream& str) const;
		};
		class Graphic: public NLComponent {
			public:
				// 虚拟输出操作符 (让就不寻常)
				virtual ostream& operator<<(ostream& str) const;
		};
		TextBlock t;
		Graphic g;
		...
		t << cout; 	// 通过virtual operator<<
								//把t打印到cout中。
								// 不寻常的语法
		g << cout; 	//通过virtual operator<<
								//把g打印到cout中。
								//不寻常的语法
		//解决办法
		class NLComponent {
			public:
				virtual ostream& print(ostream& s) const = 0;
				...
		};
		class TextBlock: public NLComponent {
			public:
				virtual ostream& print(ostream& s) const;
				...
		};
		class Graphic: public NLComponent {
			public:
				virtual ostream& print(ostream& s) const;
				...
		};
		inline ostream& operator<<(ostream& s, const NLComponent& c)
		{
				return c.print(s);
		}