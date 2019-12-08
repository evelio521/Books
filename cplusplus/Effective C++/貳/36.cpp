//不重新定义继承而来的non-virtual函数
/*
class B {
	public:
		void mf();
};

class D:public B {
	
	
};
	
	D x;
	B* pB=&x;
	pB->mf();//B:mf()
	D* pD=&x;
	pD->mf();//B:mf()
	*/
//	重新定义继承而来的non-virtual函数
#include<iostream>
using namespace std;
class B {
	public:
		void mf(){
			
			cout<<"B::mf()"<<endl;
		}
};

class D:public B {
	public:
		void mf()
		{
			cout<<"D::mf()"<<endl;
		}
	
};
	int main(){
	D x;
	B* pB=&x;
	pB->mf();//B:mf()
	D* pD=&x;
	pD->mf();//D:mf()
	//如果这样  就不能说适用于每个D都是一个B
}