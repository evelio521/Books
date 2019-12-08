//若所有的参数都需要转型，请为此采用non-member函数
class Rational_wrong{

public:
	Rational_wrong(int numerator=0,int donominator=1):n(numerator),d(donominator){}
	int  numerator()const
	{
		return n;
	}
	int donominator()const{

		return d;
	}
	const Rational_wrong operator* (const Rational_wrong& rhs)const
	{
		return Rational_wrong(n*rhs.n,d*rhs.d);
	}
private:
	int n;
	int d;
};



//解决办法是

class Rational_right{
public:
	Rational_right(int numerator=0,int donominator=1):n(numerator),d(donominator){}
	int  numerator()const
	{
		return n;
	}
	int donominator()const{

		return d;
	}

private:
	int n;
	int d;
};
const Rational_right operator*(const Rational_right &lhs,const Rational_right &rhs)
{
	return Rational_right(lhs.numerator()*rhs.numerator(),lhs.donominator()*rhs.donominator());
}
int main()
{
	Rational_wrong oneEight(1,8);
	Rational_wrong oneHalf(1,2);

	Rational_wrong result=oneHalf*oneEight;//OK
	result=result*oneEight;//OK

	result=oneHalf*2;//OK
	//result=2*oneHalf;//BAD



	Rational_right oneEight_right(1,8);
	Rational_right oneHalf_right(1,2);

	Rational_right result_right=oneHalf_right*oneEight_right;//OK
	result_right=result_right*oneEight_right;//OK

	result_right=oneHalf_right*2;//OK
	result_right=2*oneHalf_right;//OK
}



