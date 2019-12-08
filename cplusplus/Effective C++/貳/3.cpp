#include <iostream>
#include <vector>

class Rational{
	public:
		Rational(int numerator=0,int donominator=1):n(numerator),d(donominator)
		{
		}
		int  numerator()const;
		int donominator()const;
		//const Rational operator* (const Rational& rhs)const;
	private:
		int n,d;
		friend const Rational operator*(const Rational& lhs,const Rational& rhs);
};
int Rational::numerator()const
{
	return n;
}

int Rational::donominator()const
{
	return d;
}
 const Rational operator*(const Rational &lhs,const Rational &rhs)
{
	return Rational(lhs.numerator()*rhs.numerator(),lhs.donominator()*rhs.donominator());
}
//下面的两个函数需要的参数是一样的
void f1(const char* pw)
{
	
}
void f2(char const * pw)
{
	
}


 class TextBlock{
    	
    	public:
			TextBlock(const std::string str):text(str)
			{
			}
    	const char& operator[](size_t position)const
    	{
    		return text[position];
    		
    	}
		char& operator[](size_t position)
    	{
    		return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
    		
    	}
    	private:
    		std::string text;
    		
    };
    
    
    class CtextBlock{
     	
     	public:
     		CtextBlock(char *p):pText(p),lengthIsValid(false),textLength(0)
     			{
     			}
     		std::size_t length() const;
     	private:
     		char *pText;
     		mutable std::size_t textLength;//在const成员函数中不能给textLength和lengthIsValid赋值的解决方法
     		mutable bool lengthIsValid;
    };
    std::size_t CtextBlock::length()const
    	{
    		if(!lengthIsValid)
    			{
					textLength=std::strlen(pText);
    					lengthIsValid=true;
    			}
    		return textLength;
    	}
int main()
{
	char greeting[]="hello";
	char* p1=greeting;//non-const pointer non-const date
	const char* p2=greeting;//non-const pointer const date
	char* const p3=greeting;//const pointer non-const date
	const char* const p4=greeting;//const pointer const date
	
	std::vector<int> vec;
	vec.push_back(15);
	vec.push_back(25);
	
	const std::vector<int>::iterator iter=vec.begin();
	*iter=10;//不可以改变迭代器的指向
	//++iter;//wrong
	
	std::vector<int>::const_iterator citer=vec.begin();
	//*citer=10//wrong
	++citer;//可以改变指向 但是不可以改变其值
	
	Rational raa(1,8);
	Rational rab(2,8);
	Rational rac(3,8);
	
	TextBlock tb("hello");
	std::cout<<tb[0];//调用 non-const TextBlock::operator[]
    tb[0]='x';
	const TextBlock  ctb("world");
	std::cout<<ctb[0];//调用 const TextBlock::operator[]
	//ctb[0]='x';错误  不可以写const 变量

	CtextBlock cb(greeting);
	std::cout<<cb.length ();

}