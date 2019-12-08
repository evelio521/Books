/*明智而审慎的使用多重继承*/

//A 是B ,C的基类 B，C是D的基类的写法

class A {
	
};
class B:virtual public A{
	
}; 
class C:virtual public A{
	
}; 
class D: public B,public C{
	
}; //若A是虚基类 D的构造函数应该对A进行初始化


//public和private继承集合在一起

class IPerson{
	public:
		virtual ~IPerson();
		virtual string name() const=0;
		virtual string birthDate() const=0;
};
class DatabaseID{ ...};//稍后使用 细节不重要

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