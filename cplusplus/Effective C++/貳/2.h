#ifndef HEADER_IO
#include<iostream>
#endif

#ifndef HEADER_STR
#include<string>
#endif


class GamePlayer1{
	private:
		static const int numTurns=5;//����������ʽ
		int score[numTurns];
};

class GamePlayer2{
	private:
		enum {
			numTurns=5
		};
		int score[numTurns];
};
//����(��ָ��)������һ����ͷ�ļ���

//������������ʽ����һ������ ����Ҫ��#define����
const double Aspect_Ration=1.653;

//��const char* const authorName="scott meyer"��  ��Ȼ��Ҫ��#define
const std::string authorName("scott meyer");
