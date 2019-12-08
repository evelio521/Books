#include<iostream>
#include<string>
class Person{
	
	public:
		//Person(){}
		Person(std::string nstr,std::string addstr):name(nstr),address(addstr){
		}
		Person(const Person& p){
			
			name=p.name;
			address=p.address;
			std::cout<<"Person(const Person& p)"<<std::endl;
		}
		virtual ~Person(){
		}
	private:
		std::string name;
		std::string address;
};
class Student:public Person{
	
	public:
		Student(std::string schoolname,std::string schooladdress):Person(schoolname,schooladdress),schoolName(schoolname),schoolAddress(schooladdress){
			
		}
		Student(const Student& s):Person(s){
			
			schoolName=s.schoolName;
			schoolAddress=s.schoolAddress;
			std::cout<<"Student(const Student& s)"<<std::endl;
			
			
		}
		~Student(){
			
		}
		
	private:
		std::string schoolName;
		std::string schoolAddress;
};

//若这里使用pass-by-value的方式将会有一次Person copy构造函数 一次Student copy构造函数和四次string copy构造函数被调用  和6次析构函数
//若使用pass-by-reference-to-const将不会有构造函数和析构函数调用，另外还可以避免对象切个问题
bool validStudent(const Student& s){
	std::cout<<"判断Student对象是否为空"<<std::endl;
		return true;
}

int main(){
Student plato("zhangsan","JLU");
bool platoIsOK=validStudent(plato);


}