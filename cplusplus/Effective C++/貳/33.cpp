//避免遮掩继承而来的名字
/*
class Base{
	private:
		int x;
	public:
		virtual void mf1()=0;
		virtual void mf1(int){}
		virtual void mf2(){}
		void mf3(){}
		void mf3(double){}
		
		
};

class Derived:public Base{
	public:
		virtual void mf1();
		void mf3();
		void mf4();

};*/
/*
	int main()
	{
	Derived d;
	int x;
	
	d.mf1();
	d.mf1(x);//错误！Derived::mf1遮掩Base::mf1
	d.mf2();
	d.mf3();
	d.mf3(x);//错误！Derived::mf3遮掩Base::mf3
}*/
	//我们可以这样做解决上述问题
/*
	class Base{
	private:
		int x;
	public:
		virtual void mf1()=0;
		virtual void mf1(int){}
		virtual void mf2(){}
		void mf3(){}
		void mf3(double){}
		
		
};

class Derived:public Base{
	public:
		using Base::mf1;
		using Base::mf3;
		virtual void mf1(){}
		void mf3(){}
		void mf4(){}
		
};
	int main(){
	Derived d;
	int x=0;
	
	d.mf1();
	d.mf1(x);//OK！调用Base::mf1
	d.mf2();
	d.mf3();
	d.mf3(x);//OK！调用Base::mf3
}
*/
	
	//我们也可以使用转交函数实现，因为我们不想继承Base的所有函数
	class Base{
	private:
		int x;
	public:
		virtual void mf1()=0;
		virtual void mf1(int){}
		virtual void mf2(){}
		void mf3(){}
		void mf3(double){}
		
		
};

class Derived:public Base{
	public:
		virtual void mf1()
		{
			Base::mf1();//转交函数  暗自成inline
		}
	
};
int main(){
	Derived d;
	int x;
	d.mf1();
	d.mf1(x);//错误！Base::mf1被遮掩
}