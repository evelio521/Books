#include<iostream>

class Empty{//没有数据 不应该占内存

};

class HoldAnInt{//应该值需要一个int空间
	private:
		int x;
		Empty e;//应该是不需要内存
};//其实sizeof( HoldAnInt)>sizeof(int)  大多数编译器sizeof(Empty)==1
/*
//选择private继承
class HoldAnInt:private Empty {//应该值需要一个int空间
	private:
		int x;
};//sizeof( HoldAnInt)==sizeof(int) */
int main(){
	std::cout<<sizeof(Empty)<<std::endl;
		std::cout<<sizeof(HoldAnInt)<<std::endl;
		
}