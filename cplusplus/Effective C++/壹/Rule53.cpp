/*不要忽视编译信息*/
class B{
	public:
		virtual void f() const;
};

class D:public B{
	
	public:
		virtual void f();//由于未声明为const  只是对B::f的覆盖 而不是重新声明
};