/*�����еĲ�������Ҫת�ͣ���Ϊ�˲���non-member����*/

class Rational{
	
	public:
		Rational(int numerator=0,int donominator=1);
		int  numerator()const;
		int donominator()const;
		const Ration operator* (const Rational& rhs)const;
	private:
		...
};

Rational oneEight(1,8);
Rational oneHalf(1,2);

Rational result=oneHalf*oneEight;//OK
result=result*oneEight;//OK

result=oneHalf*2;//OK
result=2*oneHalf;//BAD

//����취��

class Rational{
	...
};
const Rational operator*(const Rational &lhs,const Rational &rhs)
{
	return Rational(lhs.numerator()*rhs.numerator(),lhs.donominator()*rhs.donominator());
}

Rational oneEight(1,8);
Rational oneHalf(1,2);

Rational result=oneHalf*oneEight;//OK
result=result*oneEight;//OK

result=oneHalf*2;//OK
result=2*oneHalf;//OK

