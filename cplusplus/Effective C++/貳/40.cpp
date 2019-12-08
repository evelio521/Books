//多重继承使用
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


//public和private继承集合在一起

class IPerson{
	public:
		virtual ~IPerson()｛｝;
		virtual string name() const=0;
		virtual string birthDate() const=0;
};
class DatabaseID{ 
	
};//稍后使用 细节不重要

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

//如果使用单一继承可以完成 尽量不要使用多重继承