/*宁以pass-by-reference-to-const替代pass-by-value*/

//下列代码将会非常耗时
class Person{
	
	public:
		Person();
		virtual ~Person();
		...
	private:
		std::string name;
		std::string address;
};
class Student:public Person{
	
	public:
		Student();
		~Student();
		...
	private:
		std::string schoolName;
		std::string schoolAddress;
};

bool validStudent(Student s);
Student plato;
bool platoIsOK=validStudent(plato);
//将会调用一次Student copy构造函数，一次Person Copy构造函数和四次string copy构造函数

//解决方法
bool validStudent(const Student& s);//该方法还可以防止类型的截断
