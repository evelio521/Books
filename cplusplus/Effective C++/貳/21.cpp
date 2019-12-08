#include<iostream>
class Rational{
	public:
		
		Rational(int numerator=0,int denominator=1):n(numerator),d(denominator){}

	private:
		int n,d;
		//若该用const Rational& operator*(const Rational& lhs,const Rational& rhs) 会有灾难发生
		//必须返回对象时  不要妄想返回一个reference
		friend const Rational& operator*(const Rational& lhs,const Rational& rhs);
		friend bool operator==(const Rational& lhs,const Rational& rhs);
};

//如果改用reference
/*
//绝对不要返回pointer或reference指向一个local stack 对象，或返回reference指向一个heap-allocated对象，
//或返回pointer或reference指向一个local static对象
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
*/
const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	static Rational result(lhs.n*rhs.n,lhs.d*rhs.d);//又是一个更糟糕的代码
	
	
	return result;
}
//基于上面的代码
bool operator==(const Rational& lhs,const Rational& rhs){

	if(lhs.n==rhs.n&&lhs.d==rhs.d)
		return true;
	else
		return false;
}


	
int main(){
Rational a(3,5),b(2,8),c(4,9),d(8,5);

if((a*b)==(c*d))//将永远返回真值，具体解释见教材
	{
		std::cout<<"相等"<<std::endl;
	}
else
	{
		std::cout<<"不相等"<<std::endl;
	}
}