规则5:
	/*对定制的类型转换函数保持警觉*/
	
	/*
	 * 有两种函数允许编译器进行这些的转换：
	 * 单参数构造函数（single-argument constructors）
	 * 和隐式类型转换运算符。单参数构造函数是指只用一个
	 * 参数即可以调用的构造函数。该函数可以是只定义了一个
	 * 参数，也可以是虽定义了多个参数但第一个参数以后的所有参数都有缺省值。
	 */
	 
	 class Name { // for names of things
		public:
			Name(const string& s); // 转换 string 到
														// Name
			...
		};
	class Rational { // 有理数类
		public:
			Rational(int numerator = 0, // 转换int到
			int denominator = 1); // 有理数类
			...
	};
	
	/*
	 * 隐式类型转换运算符只是一个样子奇怪的成员函数：
	 * operator 关键字，其后跟一个类型符号。你不用定
	 * 义函数的返回类型，因为返回类型就是这个函数的名字。
	 */
	 
	 class Rational {
		public:
			...
			operator double() const; // 转换Rational类成
		}; // double类型
	//在下面这种情况下，这个函数会被自动调用：
	Rational r(1, 2); // r 的值是1/2
	double d = 0.5 * r; // 转换 r 到double,
											// 然后做乘法
									
	/*
	 * 再假设你忘了为Rational对象定义operator<<。
	 * 你可能想打印操作将失败，因为没有合适的的operator<<被调用。
	 * 但是你错了。当编译器调用operator<<时，会发现没有这样的函数存在，
	 * 但是它会试图找到一个合适的隐式类型转换顺序以使得函数调用正常运行。
	 * 类型转换顺序的规则定义是复杂的，但是在现在这种情况下，编译器会发
	 * 现它们能调用Rational::operator double函数来把r转换为double类型。
	 * 所以上述代码打印的结果是一个浮点数，而不是一个有理数。这简直是一
	 * 个灾难，但是它表明了隐式类型转换的缺点：它们的存在将导致错误的发生。
	 */		
	Rational r(1, 2);
	cout << r; // 应该打印出"1/2"
 
  //解决方法是用不使用语法关键字的等同的函数来替代转换运算符
  class Rational {
		public:
			...
			double asDouble() const; //转变 Rational
		}; // 成double
	//这个成员函数能被显式调用：
		Rational r(1, 2);
		cout << r; // 错误! Rationa对象没有
							// operator<<
		cout << r.asDouble(); // 正确, 用double类型
													//打印r
													
		/*
		 * 第一个构造函数允许调用者确定数组索引的范围，例如从10到20。
		 * 它是一个两参数构造函数，所以不能做为类型转换函数。第二个
		 * 构造函数让调用者仅仅定义数组元素的个数（使用方法与内置数组
		 * 的使用相似），不过不同的是它能做为类型转换函数使用，能导致无穷的痛苦。
		 */
		template<class T>
			class Array {
				public:
					Array(int lowBound, int highBound);
					Array(int size);
					T& operator[](int index);
					...
			};
		//调用以下代码
		bool operator==( const Array<int>& lhs,
		const Array<int>& rhs);
		Array<int> a(10);
		Array<int> b(10);
		...
		for (int i = 0; i < 10; ++i)
		if (a == b[i]) { // 哎呦! "a" 应该是 "a[i]"
			do something for when
			a[i] and b[i] are equal;
		}
		else {
			do something for when they are not;
}
		//会执行一下代码
		for (int i = 0; i < 10; ++i)
    if (a == static_cast< Array<int> >(b[i])) ...
    	
    //解决办法 使用关键词explicit
    template<class T>
		class Array {
			public:
				...
				explicit Array(int size); // 注意使用"explicit"
				...
		};
		Array<int> a(10); // 正确, explicit 构造函数
											// 在建立对象时能正常使用
		Array<int> b(10); // 也正确
		if (a == b[i]) ... // 错误! 没有办法
											// 隐式转换
											// int 到 Array<int>
		if (a == Array<int>(b[i])) ... // 正确,显式从int到
																	// Array<int>转换
																	// （但是代码的逻辑
																// 不合理）
		if (a == static_cast< Array<int> >(b[i])) ...
									// 同样正确，同样
										// 不合理
		if (a == (Array<int>)b[i]) ... //C风格的转换也正确，
															// 但是逻辑
													// 依旧不合理
													
		//另外的解决办法
		template<class T>
		class Array {
			public:
				class ArraySize { // 这个类是新的
					public:
						ArraySize(int numElements): theSize(numElements) {}
						int size() const { return theSize; }
					private:
						int theSize;
					};
				Array(int lowBound, int highBound);
				Array(ArraySize size); // 注意新的声明
				...
		};
    /*
     * 你的编译器要求用int参数调用Array<int>里的构造函数，但是没有这样的构造函数。
     * 编译器意识到它能从int参数转换成一个临时ArraySize对象，ArraySize对象只是
     * Array<int>构造函数所需要的，这样编译器进行了转换。函数调用（及其后的对象建立）也就成功了。
     */
     
     bool operator==( const Array<int>& lhs,const Array<int>& rhs);
			Array<int> a(10);
			Array<int> b(10);
			...
		for (int i = 0; i < 10; ++i)
				if (a == b[i]) ... // 哎呦! "a" 应该是 "a[i]";
														// 现在是一个错误。