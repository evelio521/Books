#include<iostream>
#include<string>
int main()
{
	//new 和delete 最好成对的出现
	std::string *str1=new std::string("sdfgd");
	std::string *str2=new std::string[100];
		
  delete str1;
  delete [] str2;
  //下列做法不可取   导致错误率增高
  typedef std::string Address[4];
  std::string* pa=new Address;	
  delete [] pa;
}