//��operator= ����һ��reference to *this
//�ⲻ��һ��ǿ�ƹ涨 ��������û�������
#include<iostream>
class Widget{
	public:
		Widget& operator=(const Widget& rhs)
		{
			std::cout<<"operator= ����һ��reference to *this"<<std::endl;
			return *this;
		}
};

int main()
{
	
	Widget w1,w2,w3;
	w1=w2=w3;
}