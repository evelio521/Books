#include<iostream>
//�������Ҹ�ֵ������ 
class Bitmap{

public:
	Bitmap()
	{
	}
	
};
class Widget{
	public:
		Widget(Bitmap *pbb):pb(pbb)
		{
		}
		Widget& operator=(const Widget& rhs)
		{
		
			//std::cout<<"operator= ����һ��reference to *this"<<std::endl;
			Bitmap *pOrig=pb;
			pb=new Bitmap(*rhs.pb);
			delete pOrig;
			return *this;
		}
	private:
		Bitmap *pb;
		
};

int main()
{
	Bitmap *pc=new Bitmap();
	Widget w1(pc),w2(pc);
	w1=w2;
}