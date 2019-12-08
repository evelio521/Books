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

//������ʹ��pass-by-value�ķ�ʽ������һ��Person copy���캯�� һ��Student copy���캯�����Ĵ�string copy���캯��������  ��6����������
//��ʹ��pass-by-reference-to-const�������й��캯���������������ã����⻹���Ա�������и�����
bool validStudent(const Student& s){
	std::cout<<"�ж�Student�����Ƿ�Ϊ��"<<std::endl;
		return true;
}

int main(){
Student plato("zhangsan","JLU");
bool platoIsOK=validStudent(plato);


}