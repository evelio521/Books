#include <iostream>
#include<string>
void printDouble(const double& rd)
{
	std::cout<<rd;//使用前无需检查
}
void printInt(const int* rd)
{
	if(rd)
		std::cout<<*rd;//使用前必需检查
}
int main()
{
	std::string s("xzcxzc");
	std::string &rs=s;//reference必须指向一个对象
	std::string ps;//pointer可以不只想一个对象 但是很危险
	
	std::string s1("nacy");
	std::string s2("scott");
	
	std::string& rss=s1;
	std::string* pss=&s1;
			
	rss=s2;//s1也改变值
	pss=&s2;//s1不变值
	
	std::cout<<s1<<std::endl;
	std::cout<<s2<<std::endl;
		
}