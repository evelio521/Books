/*���Բ����¶���̳ж�����non-virtual����*/

//�����¶���̳ж�����non-virtual����
class B {
	public:
		void mf();
};

class D:public B {...};
	
	D x;
	B* pB=&x;
	pB->mf();//B:mf()
	D* pD=&x;
	pD->mf();//B:mf()
	
//	���¶���̳ж�����non-virtual����

class B {
	public:
		void mf();
};

class D:public B {
	puhlic:
		void mf();
	...
};
	
	D x;
	B* pB=&x;
	pB->mf();//B:mf()
	D* pD=&x;
	pD->mf();//D:mf()
	