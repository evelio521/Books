规则:22
	/*考虑用操作符的复合形式（op=）取代其单独形式（op）*/
	
	class Rational {
		public:
			...
			Rational& operator+=(const Rational& rhs);
			Rational& operator-=(const Rational& rhs);
		};
	// operator+ 根据operator+=实现;
	//有关为什么返回值是const的解释，
	//参见Effective C++条款21 和 109页 的有关实现的警告
		const Rational operator+(const Rational& lhs,const Rational& rhs)
		{
			return Rational(lhs) += rhs;
		}
		// operator- 根据 operator -= 来实现
		const Rational operator-(const Rational& lhs,const Rational& rhs)
		{
			return Rational(lhs) -= rhs;
		}

		//如果你不介意把所有的operator的单独形式放在全局域里，
		//那就可以使用模板来替代单独形式的函数的编写：
		template<class T>
		const T operator+(const T& lhs, const T& rhs)
		{
				return T(lhs) += rhs; // 参见下面的讨论
		}
		template<class T>
		const T operator-(const T& lhs, const T& rhs)
		{
				return T(lhs) -= rhs; // 参见下面的讨论
		}


		//效率方面的问题。
		//第一、总的来说operator的赋值形式比其单独形式效率更高，
				//因为单独形式要返回一个新对象，从而在临时对象的构造和
				//释放上有一些开销（参见条款M19和条款M20，还有Effective 
				//C++条款23）。operator的赋值形式把结果写到左边的参数里，
				//因此不需要生成临时对象来容纳operator的返回值。
		//第二、提供operator的赋值形式的同时也要提供其标准形式，
				//允许类的客户端在便利
				//与效率上做出折衷选择。也就是说，客户端可以决定是这样编写：
			Rational a, b, c, d, result;
			...
			result = a + b + c + d; // 可能用了3个临时对象
			// 每个operator+ 调用使用1个
			还是这样编写：
			result = a; //不用临时对象
			result += b; //不用临时对象
			result += c; //不用临时对象
			result += d; //不用临时对象
			
			
			template<class T>
			const T operator+(const T& lhs, const T& rhs)
			{
					T result(lhs); // 拷贝lhs 到 result中
					return result += rhs; // rhs与它相加并返回结果
			}
		//这个模板几乎与前面的程序相同，但是它们之间还是存在重要
		//的差别。第二个模板包含一个命名对象，result。这个命名对
		//象意味着不能在operator+ 里使用返回值优化（参见条款M20）。
		//第一种实现方法总可以使用返回值优化，所以编译器为其生成优
		//化代码的可能就会更大。