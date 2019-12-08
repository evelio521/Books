/*必须返回对象时，别妄想返回reference*/

class Rational{
	public:
		Rational(int numerator=0,int denominator=1);
		...
	private:
		int n,d;
		friend const Rational operator*(const Rational& lhs,const Rational& rhs);
};

//如果改用reference

const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	Rational result(lhs.n*rhs.n,lhs.d*rhs.d);//糟糕的代码
	return result;
}

const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	Rational *result=new Rational(lhs.n*rhs.n,lhs.d*rhs.d);//更糟糕的代码
	return *result;
}

const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	static Reational result;//又是一个更糟糕的代码
	
	result=...;

	return result;
}
//基于上面的代码
bool operator==(const Rational& lhs,const Rational& rhs);

Rational a,b,c,d;

if((a*b)==(c*d))//将永远返回真值，具体解释见教材
	{
	}
else
	{
	}
	
