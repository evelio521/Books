//���ؼ̳�ʹ��
/*
class A {
	public:
		A(int Aint):val(Aint){}
	private:
		int val;
	
};
class B:virtual public A{
	public:
		B(int Bint):A(Bint),val(Bint){}
	private:
		int val;
}; 
class C:virtual public A{
	public:
		C(int Cint):A(Cint),val(Cint){}
	private:
		int val;
}; 
class D: public B,public C{
	public:
		D(int Dint):A(Dint),B(Dint),C(Dint),val(Dint){}
	private:
		int val;
}; 

int main(){
	B b(6);
	C c(7);	
	D d(5);
}*/


//public��private�̳м�����һ��

class IPerson{
	public:
		virtual ~IPerson()����;
		virtual string name() const=0;
		virtual string birthDate() const=0;
};
class DatabaseID{ 
	
};//�Ժ�ʹ�� ϸ�ڲ���Ҫ

class PersonInfo{
	public:
		explicit PersonInfo(DatabaseID pid);
		virtual ~PersonInfo();
		virtual const char* theName() const;
		virtual const char* theBirthData()const;
		virtual const char* valueDelimOpen()const;
		virtual const char* valueDelimClose()const;
		
};

class CPerson:public IPerson,private PersonInfo{
	
	public:
		CPerson(DatabaseID pid):PersonInfo(pid){}
		virtual string name() const
		{
			return PersonInfo::theName;
		}
		virtual string birthDate() const
		{
			return PersonInfo::birthDate;
		}
	private:
		const char* valueDelimOpen()const {return " ";}
		const char* valueDelimClose()const{return " ";}
		
};

//���ʹ�õ�һ�̳п������ ������Ҫʹ�ö��ؼ̳�