/*��Ҫ����ת��ʱ��Ϊģ�涨��ǳ�Ա����*/

//templateʵ���Ƶ������дӲ�����ʽ������ת�����뿼��
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
Rational<int> reault=oneHalf*2;//�������


//�������

template<typename T> class Rational;//����Rational template
template<typename T> const Rational<T> doMultiply( const Rational<T>& lhs,const Rational<T>& rhs);//����helper tempalte

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
 