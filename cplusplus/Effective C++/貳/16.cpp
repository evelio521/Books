#include<iostream>
#include<string>
int main()
{
	//new ��delete ��óɶԵĳ���
	std::string *str1=new std::string("sdfgd");
	std::string *str2=new std::string[100];
		
  delete str1;
  delete [] str2;
  //������������ȡ   ���´���������
  typedef std::string Address[4];
  std::string* pa=new Address;	
  delete [] pa;
}