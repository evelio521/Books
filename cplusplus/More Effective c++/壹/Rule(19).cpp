规则:19
	/*了解临时对象的来源*/
	
	//在C++中真正的临时对象是看不见的，它们不出现在你的源代码中。
	//建立一个没有命名的非堆（non-heap）对象会产生临时对象。这种
	//未命名的对象通常在两种条件下产生：为了使函数成功调用而进行
	//隐式类型转换和函数返回对象时。理解如何和为什么建立这些临时
	//对象是很重要的，因为构造和释放它们的开销对于程序的性能来说
	//有着不可忽视的影响。

	//首先考虑为使函数成功调用而建立临时对象这种情况。当传送给函数
	//的对象类型与参数类型不匹配时会产生这种情况。例如一个函数，它
	//用来计算一个字符在字符串中出现的次数：
		// 返回ch在str中出现的次数
		size_t countChar(const string& str, char ch);
		char buffer[MAX_STRING_LEN];
		char c;
		// 读入到一个字符和字符串中，用setw
		// 避免缓存溢出，当读取一个字符串时
		cin >> c >> setw(MAX_STRING_LEN) >> buffer;
		cout << "There are " << countChar(buffer, c)
		<< " occurrences of the character " << c
		<< " in " << buffer << endl;
		///看一下countChar的调用。第一个被传送的参数是字符数组，
		//但是对应函数的正被绑定的参数的类型是const string&。仅
		//当消除类型不匹配后，才能成功进行这个调用，你的编译器很
		//乐意替你消除它，方法是建立一个string类型的临时对象。通
		//过以buffer做为参数调用string的构造函数来初始化这个临时
		//对象。countChar的参数str被绑定在这个临时的string对象上。
		//当countChar返回时，临时对象自动释放。
		
		//仅当通过传值（by value）方式传递对象或传递常量引用
		//（reference-to-const）参数时，才会发生这些类型转换。
		//当传递一个非常量引用（reference-to-non-const）参数对象，
		//就不会发生。考虑一下这个函数：
		void uppercasify(string& str); // 把str中所有的字符
		// 改变成大写
		//在字符计数的例子里，能够成功传递char数组到countChar中，
		//但是在这里试图用char数组调用upeercasify函数，则不会成功：
		char subtleBookPlug[] = "Effective C++";
		uppercasify(subtleBookPlug); // 错误!
		
		
		
		//建立临时对象的第二种环境是函数返回对象时。例如operator+必须返回一个对象，以表示它的两个操作数的和（参见Effective C++ 条款23）。例如给定一个类型Number，这种类型的operator+被这样声明：
			const Number operator+(const Number& lhs,const Number& rhs);
		//这个函数的返回值是临时的，因为它没有被命名；它只是函数的返回值。
		//你必须为每次调用operator+构造和释放这个对象而付出代价。
		//（有关为什么返回值是const的详细解释，参见Effective C++条款21）
		//通常你不想付出这样的开销。对于这种函数，你可以切换到operator=，
		//而避免开销。条款M22告诉我们进行这种转换的方法。不过对于大多数返
		//回对象的函数来说，无法切换到不同的函数，从而没有办法避免构造和释
		//放返回值。至少在概念上没有办法避免它。然而概念和现实之间又一个黑
		//暗地带，叫做优化，有时你能以某种方法编写返回对象的函数，以允许你
		//的编译器优化临时对象。这些优化中，最常见和最有效的是返回值优化，
		//这是条款M20的内容。