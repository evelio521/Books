#include <iostream>
#include<string>
void printDouble(const double& rd)
{
	std::cout<<rd;//ʹ��ǰ������
}
void printInt(const int* rd)
{
	if(rd)
		std::cout<<*rd;//ʹ��ǰ������
}
int main()
{
	std::string s("xzcxzc");
	std::string &rs=s;//reference����ָ��һ������
	std::string ps;//pointer���Բ�ֻ��һ������ ���Ǻ�Σ��
	
	std::string s1("nacy");
	std::string s2("scott");
	
	std::string& rss=s1;
	std::string* pss=&s1;
			
	rss=s2;//s1Ҳ�ı�ֵ
	pss=&s2;//s1����ֵ
	
	std::cout<<s1<<std::endl;
	std::cout<<s2<<std::endl;
		
}