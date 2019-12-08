//测试静态常量成员才类内部初始化
#include<iostream>
using namespace std;

template <typename>
class testClass{
	
	public:
		static const int _datai=5;
		static const long _datal=3L;
		static const char _datac='c';
	
};

int main(){
	
	cout<<testClass<int>::_datai<<endl;//5
		cout<<testClass<long>::_datal<<endl;//3
			cout<<testClass<char>::_datac<<endl;//c
}