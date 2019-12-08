/*需要类型转换时请为模版定义非成员函数*/

//template实参推导过程中从不将隐式的类型转换纳入考虑
template<typename T>
class Rational{
	public:
		Rational(const T& numerator=0,const T& denominator=1);
		const T numerator()const;
		const T denominator()const;
		...
};
 template<typename T>
 const Rational<T> operator*(const Rational<T>& lhs,const Rational<T>& rhs
 {
 	...
}

Rational<int> oneHalf(1,2);
Rational<int> reault=oneHalf*2;//编译错误


//解决方法

template<typename T> class Rational;//声明Rational template
template<typename T> const Rational<T> doMultiply( const Rational<T>& lhs,const Rational<T>& rhs);//声明helper tempalte

template<typename T>
class Rational{
	public:
		Rational(const T& numerator=0,const T& denominator=1);
		const T numerator()const;
		const T denominator()const;
		friend const Rational<T> operator*(const Rational<T>& lhs,const Rational<T>& rhs)
		{
			return doMultiply(lhs,rhs);
		}
		...
};

template<typename T> const Rational<T> doMultiply( const Rational<T>& lhs,const Rational<T>& rhs)
{
	return Rational<T>(lhs.numerator()*rhs.numerator(),lhs.denominator()*rhs.denominator());
}
 