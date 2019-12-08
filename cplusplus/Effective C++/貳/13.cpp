#include<iostream>
#include<string>
#include<memory> //auto_ptr 的头文件
#include "d:\boost_1_52_0\boost\shared_ptr.hpp"
class Base{
	public:
		Base(){}
		virtual ~Base(){}
		Base(const Base& b);
		Base& operator=(const Base& b);
	private:
		std::string str;
		int ival;//当增加成员变量时  要修改上面的的四个函数
						//赋值的时候不要忘记每一个成分
		
};
Base::Base(const Base& b):str(b.str),ival(b.ival){
	
	
}
Base& Base::operator=(const Base& b){
	
	str=b.str;
	ival=b.ival;
	return *this;
}



int main()
{
	std::auto_ptr<Base> b1(new Base());
	std::auto_ptr<Base> b2(b1);
	/*	
	tr1::shared_ptr<Base> b3(new Base());
	tr1::shared_ptr<Base> b4(b3);
		*/
	//auto_ptr不会降低程序的效率，但auto_ptr不适用于数组，auto_ptr根本不可以大规模使用。
	//但是要注意使用中的一个陷阱,那就是指针的托管权是会转移的。
	//例如在上例中，如果 auto_ptr<TC> pTC(new TC); auto_ptr<TC> pTC1=pTC; 
	//那么,pTC1将拥有该指针，而pTC没有了，如果再用pTC去引用，必然导致内存错误。 
}