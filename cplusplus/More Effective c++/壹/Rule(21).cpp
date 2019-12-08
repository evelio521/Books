规则:21
	/*以重载技术避免隐式类型转换*/
	
	//让编译器完成这种类型转换是确实是很方便，但是建立临时对象进行类型转换
	//工作是有开销的，而我们不想承担这种开销。就象大多数人只想从政府那里受
	//益而不想为此付出一样，大多数C++程序员希望进行没有临时对象开销的隐式
	//类型转换。我们的目的不是真的要进行类型转换，而是用UPint和int做为参数
	//调用operator+。隐式类型转换只是用来达到目的的手段，但是我们不要混淆
	//手段与目的
	
	
	//以下的代码不是很好 ，存在隐式转换
	class UPInt { // unlimited precision
		public: // integers 类
			UPInt();
			UPInt(int value);
			...
	};
		//有关为什么返回值是const的解释，参见Effective C++ 条款21
	const UPInt operator+(const UPInt& lhs, const UPInt& rhs);
	UPInt upi1, upi2;
	...
	UPInt upi3 = upi1 + upi2;
		
	//现在考虑下面这些语句：
	upi3 = upi1 + 10;//ok
	upi3 = 10 + upi2;//ok
	
	//改进的算法
	const UPInt operator+(const UPInt& lhs, // add UPInt
	const UPInt& rhs); // and UPInt
	const UPInt operator+(const UPInt& lhs, // add UPInt
	int rhs); // and int
	const UPInt operator+(int lhs, // add int and
	const UPInt& rhs); // UPInt
	UPInt upi1, upi2;
	...
	UPInt upi3 = upi1 + upi2; // 正确,没有由upi1 或 upi2
	// 生成的临时对象
	upi3 = upi1 + 10; // 正确, 没有由upi1 or 10
	// 生成的临时对象
	upi3 = 10 + upi2; //正确, 没有由10 or upi2
	//生成的临时对象。
		