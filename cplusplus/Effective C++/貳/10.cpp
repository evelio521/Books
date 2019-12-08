//令operator= 返回一个reference to *this
//这不是一个强制规定 但是你最好还是遵守
#include<iostream>
class Widget{
	public:
		Widget& operator=(const Widget& rhs)
		{
			std::cout<<"operator= 返回一个reference to *this"<<std::endl;
			return *this;
		}
};

int main()
{
	
	Widget w1,w2,w3;
	w1=w2=w3;
}