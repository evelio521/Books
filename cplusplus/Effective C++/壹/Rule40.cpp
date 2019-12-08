/*���Ƕ�������ʹ�ö��ؼ̳�*/

//A ��B ,C�Ļ��� B��C��D�Ļ����д��

class A {
	
};
class B:virtual public A{
	
}; 
class C:virtual public A{
	
}; 
class D: public B,public C{
	
}; //��A������� D�Ĺ��캯��Ӧ�ö�A���г�ʼ��


//public��private�̳м�����һ��

class IPerson{
	public:
		virtual ~IPerson();
		virtual string name() const=0;
		virtual string birthDate() const=0;
};
class DatabaseID{ ...};//�Ժ�ʹ�� ϸ�ڲ���Ҫ

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