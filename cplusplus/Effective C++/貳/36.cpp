//�����¶���̳ж�����non-virtual����
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
//	���¶���̳ж�����non-virtual����
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
	//�������  �Ͳ���˵������ÿ��D����һ��B
}