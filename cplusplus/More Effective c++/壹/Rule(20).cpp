规则:20
  /*协助完成返回值优化*/
  
  class Rational {
		public:
			Rational(int numerator = 0, int denominator = 1);
			...
			int numerator() const;
			int denominator() const;
	};
		// 有关为什么返回值是const的解释，参见条款M6,
		const Rational operator*(const Rational& lhs,const Rational& rhs);
		
		// 一种不合理的避免返回对象的方法
		const Rational * operator*(const Rational& lhs,
		const Rational& rhs);
		Rational a = 10;
		Rational b(1, 2);
		Rational c = *(a * b); //你觉得这样很“正常”么？
		
		//一种危险的(和不正确的)方法，用来避免返回对象
		const Rational& operator*(const Rational& lhs,
		const Rational& rhs);
		Rational a = 10;
		Rational b(1, 2);
		Rational c = a * b; // 看上去很合理
				
		// 另一种危险的方法 (和不正确的)方法，用来
		// 避免返回对象
		const Rational& operator*(const Rational& lhs,const Rational& rhs)
		{
		Rational result(lhs.numerator() * rhs.numerator(),
		lhs.denominator() * rhs.denominator());
		return result;//WQ加注 返回时，其指向的对象已经不存在了
		}
		
		// 一种高效和正确的方法，用来实现
		// 返回对象的函数
		const Rational operator*(const Rational& lhs,
		const Rational& rhs)
		{
				return Rational(lhs.numerator() * rhs.numerator(),
				lhs.denominator() * rhs.denominator());
		}
		
		Rational a = 10;
		Rational b(1, 2);
		Rational c = a * b; // 在这里调用operator*
		//编译器就会被允许消除在operator*内的临时变量和operator*返
		//回的临时变量。它们能在为目标c分配的内存里构造return表达式
		//定义的对象。如果你的编译器这样去做，调用operator*的临时对
		//象的开销就是零：没有建立临时对象。你的代价就是调用一个构造
		//函数――建立c时调用的构造函数
		
		// the most efficient way to write a function returning
		// an object
			inline const Rational operator*(const Rational& lhs,
			const Rational& rhs)
			{
					return Rational(lhs.numerator() * rhs.numerator(),
					lhs.denominator() * rhs.denominator());
			}