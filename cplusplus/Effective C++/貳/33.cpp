//�������ڼ̳ж���������
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
	d.mf1(x);//����Derived::mf1����Base::mf1
	d.mf2();
	d.mf3();
	d.mf3(x);//����Derived::mf3����Base::mf3
}*/
	//���ǿ��������������������
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
	d.mf1(x);//OK������Base::mf1
	d.mf2();
	d.mf3();
	d.mf3(x);//OK������Base::mf3
}
*/
	
	//����Ҳ����ʹ��ת������ʵ�֣���Ϊ���ǲ���̳�Base�����к���
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
			Base::mf1();//ת������  ���Գ�inline
		}
	
};
int main(){
	Derived d;
	int x;
	d.mf1();
	d.mf1(x);//����Base::mf1������
}