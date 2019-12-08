#include<iostream>
#include<string>

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

class Derived:public Base{
	public:
		Derived(){}
		virtual ~Derived(){}
		Derived(const Derived& d);
		Derived& operator=(const Derived& d);
	private:
		std::string str;
		int ival;//赋值的时候不要忘记每一个成分 继承类还要给基类赋值
	
};
Derived::Derived(const Derived& d):Base(d),str(d.str),ival(d.ival){//切记 不要忘记给基类初始化
	
	
}
Derived& Derived::operator=(const Derived& d){
	
	Base::operator=(d);//切记 不要忘记给基类初始化
	str=d.str;
	ival=d.ival;
	return *this;
}

int main()
{
	Base b1,b2;
	Derived d1,d2;
	b1=d1;
}