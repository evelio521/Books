规则30:
	/*proxy classes*/
	//实现二维数组
	class Array2D {
		public:
			class Array1D {
				public:
					T& operator[](int index);
					const T& operator[](int index) const;
					...
		};
			Array1D operator[](int index);
			const Array1D operator[](int index) const;
			...
	};
	//现在，它合法了：
		Array2D<float> data(10, 20);
		...
		cout << data[3][6]; // fine
		
	//区分operator  []的读写动作
	class String { // reference-counted strings;
		public: // see Item 29 for details
			class CharProxy { // proxies for string chars
				public:
					CharProxy(String& str, int index); // creation
					CharProxy& operator=(const CharProxy& rhs); // lvalue
					CharProxy& operator=(char c); // uses
					operator char() const; // rvalue
					// use
				private:
					String& theString; // string this proxy pertains to
					int charIndex; // char within that string
					// this proxy stands for
			};
				// continuation of String class
			const CharProxy
			operator[](int index) const; // for const Strings
			CharProxy operator[](int index); // for non-const Strings
			...
			friend class CharProxy;
		private:
			RCPtr<StringValue> value;
	};

	String s1, s2; // reference-counted strings
	// using proxies
	...
	cout << s1[5]; // still legal, still works
 	//表达式s1[5]返回的是一CharProxy对象。没有为这样的对象定义输出流操作，
 	//所以编译器努力地寻找一个隐式的类型转换以使得operator<<调用成功（见Item M5）。
 	//它们找到一个：在CahrProxy类内部申明了一个隐式转换到char的操作。于是
 	//自动调用这个转换操作，结果就是CharProxy类扮演的字符被打印输出了。这个
 	//CharProxy到char的转换是所有代理对象作右值使用时发生的典型行为。
	s2[5] = 'x'; // also legal, also works
	//和前面一样，表达式s2[5]返回的是一个CharProxy对象，但这次它是赋值操作的目标。
	//由于赋值的目标是CharProxy类，所以调用的是CharProxy类中的赋值操作。
	//这至关重要，因为在CharProxy的赋值操作中，我们知道被赋值的CharProxy对象是作
	//左值使用的。因此，我们知道proxy类扮演的字符是作左值使用的，必须执行一些必要
	//的操作以实现字符的左值操作。
	s1[3] = s2[8]; // of course it's legal,of course it works
 	//调用作用于两个CharProxy对象间的赋值操作，在此操作内部，
 	//我们知道左边一个是作左值，右边一个作右值。
 
 	const String::CharProxy String::operator[](int index) const
		{
			return CharProxy(const_cast<String&>(*this), index);
		}
	String::CharProxy String::operator[](int index)
		{
			return CharProxy(*this, index);
		}
	String::CharProxy::CharProxy(String& str, int index)
		: theString(str), charIndex(index) {}
	String::CharProxy::operator char() const
		{
			return theString.value->data[charIndex];
		}

	String::CharProxy&
	String::CharProxy::operator=(const CharProxy& rhs)
	{
		// if the string is sharing a value with other String objects,
		// break off a separate copy of the value for this string only
		if (theString.value->isShared()) {
			theString.value = new StringValue(theString.value->data);
		}
		// now make the assignment: assign the value of the char
		// represented by rhs to the char represented by *this
		theString.value->data[charIndex] =
		rhs.theString.value->data[rhs.charIndex];
		return *this;
		}
	String::CharProxy& String::CharProxy::operator=(char c)
		{
			if (theString.value->isShared()) {
				theString.value = new StringValue(theString.value->data);
				}
			theString.value->data[charIndex] = c;
			return *this;
		}
		
		
		
		//使用代理类的限制
		//如果String::operator[]返回一个CharProxy而不是char &，下面的代码将不能编译：
		String s1 = "Hello";
		char *p = &s1[1]; // error!
		//表达式s1[1]返回一个CharProxy，于是“=”的右边是一个CharProxy *。
		//没有从CharProxy *到char *的转换函数，所以p的初始化过程编译失败了。
		//通常，取proxy对象地址的操作与取实际对象地址的操作得到的指针，其类型是不同的。
		
		//要消除这个不同，你需要重载CharProxy类的取地址运算：
		class String {
			public:
				class CharProxy {
					public:
						...
						char * operator&();
						const char * operator&() const;
					...
				};
				...
	 };
	 
	 const char * String::CharProxy::operator&() const
	{
			return &(theString.value->data[charIndex]);
	}
	char * String::CharProxy::operator&()
	{
		// make sure the character to which this function returns
		// a pointer isn't shared by any other String objects
		if (theString.value->isShared()) {
			theString.value = new StringValue(theString.value->data);
		}
		// we don't know how long the pointer this function
		// returns will be kept by clients, so the StringValue
		// object can never be shared
		theString.value->markUnshareable();
		return &(theString.value->data[charIndex]);
	}
	
	
	template<class T> // reference-counted array
	class Array { // using proxies
		public:
			class Proxy {
				public:
					Proxy(Array<T>& array, int index);
					Proxy& operator=(const T& rhs);
					operator T() const;
					...
			};
		const Proxy operator[](int index) const;
		Proxy operator[](int index);
	...
	};
	//看一下这个数组可能被怎样使用：
		Array<int> intArray;
		...
		intArray[5] = 22; // fine
		intArray[5] += 5; // error!
		++intArray[5]; // error!
		//解决办法就是为代理类手动添加+= ++操作符

	
		//我们将定义一个Rational类，然后使用前面看到的Array模板：
	class Rational {
		public:
			Rational(int numerator = 0, int denominator = 1);
			int numerator() const;
			int denominator() const;
			...
	};
	Array<Rational> array;
	//这是我们所期望的使用方式，但我们很失望：
	cout << array[4].numerator(); // error!
	int denom = array[22].denominator(); // error!

	//另一个proxy对象替代实际对象失败的情况是作为非const的引用传给函数：
	void swap(char& a, char& b); // swaps the value of a and b
	String s = "+C+"; // oops, should be "C++"
	swap(s[0], s[1]); // this should fix the
	// problem, but it won't
	// compile
	
	
	class TVStation {
		public:
			TVStation(int channel);
			...
	};
		void watchTV(const TVStation& station, float hoursToWatch);
		//借助于int到TVStation的隐式类型转换（见Item M5），我们可以这么做：
		watchTV(10, 2.5); // watch channel 10 for
		// 2.5 hours
		//然而，当使用那个用proxy类区分operator[]作左右值的带引用计数的数组模板时，我们就不能这么做了：
		Array<int> intArray;
		intArray[4] = 10;
		watchTV(intArray[4], 2.5); // error! no conversion
		// from Proxy<int> to
		// TVStation