#ifndef HEADER_IO
#include<iostream>
#endif

#ifndef HEADER_STR
#include<string>
#endif


class GamePlayer1{
	private:
		static const int numTurns=5;//常量的声明式
		int score[numTurns];
};

class GamePlayer2{
	private:
		enum {
			numTurns=5
		};
		int score[numTurns];
};
//常量(或指针)的声明一般在头文件中

//尽量用这种形式声明一个常量 而不要用#define声明
const double Aspect_Ration=1.653;

//比const char* const authorName="scott meyer"好  当然不要用#define
const std::string authorName("scott meyer");
