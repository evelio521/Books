#include "1.h"

#ifndef HEADER_IO
#include<iostream>
#endif

#ifndef HEADER_STR
#include<string>
#endif
//尽量不要用#define的形式 
template<typename T>
inline T callMax(const T& a,const T& b)
{	
	return a>b?a:b;

}
int main()
{
	std::cout<<"Aspect_Ration："<<Aspect_Ration<<std::endl;
	std::cout<<"authorName："<<authorName<<std::endl;
	GamePlayer1 gp1;
	GamePlayer2 gp2;
	const int a=5,b=6;
	std::cout<<callMax(a,b)<<std::endl;
		
	return 0;
}