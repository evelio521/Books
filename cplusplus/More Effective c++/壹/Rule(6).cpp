规则6:
	/*M6：自增(increment)、自减(decrement)操作符前缀形式与后缀形式的区别*/
	class UPInt { // "unlimited precision int"
		public:
			UPInt& operator++(); // ++ 前缀
			const UPInt operator++(int); // ++ 后缀
			UPInt& operator--(); // -- 前缀
			const UPInt operator--(int); // -- 后缀
			UPInt& operator+=(int); // += 操作符，UPInts
			// 与ints 相运算
			...
		};
			UPInt i;
			++i; // 调用 i.operator++();
			i++; // 调用 i.operator++(0);
			--i; // 调用 i.operator--();
			i--; // 调用 i.operator--(0);
			
			// 前缀形式：增加然后取回值
			UPInt& UPInt::operator++()
			{
				*this += 1; // 增加
				return *this; // 取回值
			}
			// postfix form: fetch and increment
			const UPInt UPInt::operator++(int)
			{
				UPInt oldValue = *this; // 取回值
				++(*this); // 增加
				return oldValue; // 返回被取回的值
			}
			
			/*
			 * 结论，如果仅为了提高代码效率，UPInt的调用者应该尽量使用前缀increment，
			 * 少用后缀increment，除非确实需要使用后缀increment。让我们明确一下，当
			 * 处理用户定义的类型时，尽可能地使用前缀increment，因为它的效率较高。
			 */