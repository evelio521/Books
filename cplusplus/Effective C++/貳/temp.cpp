#include<iostream>

class Empty{//û������ ��Ӧ��ռ�ڴ�

};

class HoldAnInt{//Ӧ��ֵ��Ҫһ��int�ռ�
	private:
		int x;
		Empty e;//Ӧ���ǲ���Ҫ�ڴ�
};//��ʵsizeof( HoldAnInt)>sizeof(int)  �����������sizeof(Empty)==1
/*
//ѡ��private�̳�
class HoldAnInt:private Empty {//Ӧ��ֵ��Ҫһ��int�ռ�
	private:
		int x;
};//sizeof( HoldAnInt)==sizeof(int) */
int main(){
	std::cout<<sizeof(Empty)<<std::endl;
		std::cout<<sizeof(HoldAnInt)<<std::endl;
		
}