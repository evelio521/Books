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
		int ival;//�����ӳ�Ա����ʱ  Ҫ�޸�����ĵ��ĸ�����
						//��ֵ��ʱ��Ҫ����ÿһ���ɷ�
		
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
		int ival;//��ֵ��ʱ��Ҫ����ÿһ���ɷ� �̳��໹Ҫ�����ำֵ
	
};
Derived::Derived(const Derived& d):Base(d),str(d.str),ival(d.ival){//�м� ��Ҫ���Ǹ������ʼ��
	
	
}
Derived& Derived::operator=(const Derived& d){
	
	Base::operator=(d);//�м� ��Ҫ���Ǹ������ʼ��
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