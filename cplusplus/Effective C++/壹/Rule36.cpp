/*绝对不重新定义继承而来的non-virtual函数*/

//不重新定义继承而来的non-virtual函数
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
	
//	重新定义继承而来的non-virtual函数

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
	