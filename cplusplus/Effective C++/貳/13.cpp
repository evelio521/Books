#include<iostream>
#include<string>
#include<memory> //auto_ptr ��ͷ�ļ�
#include "d:\boost_1_52_0\boost\shared_ptr.hpp"
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



int main()
{
	std::auto_ptr<Base> b1(new Base());
	std::auto_ptr<Base> b2(b1);
	/*	
	tr1::shared_ptr<Base> b3(new Base());
	tr1::shared_ptr<Base> b4(b3);
		*/
	//auto_ptr���ή�ͳ����Ч�ʣ���auto_ptr�����������飬auto_ptr���������Դ��ģʹ�á�
	//����Ҫע��ʹ���е�һ������,�Ǿ���ָ����й�Ȩ�ǻ�ת�Ƶġ�
	//�����������У���� auto_ptr<TC> pTC(new TC); auto_ptr<TC> pTC1=pTC; 
	//��ô,pTC1��ӵ�и�ָ�룬��pTCû���ˣ��������pTCȥ���ã���Ȼ�����ڴ���� 
}