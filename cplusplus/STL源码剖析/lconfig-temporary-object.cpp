//本例测试仿函数用于for_each()情形
#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;
template<typename T>
class print{
	
	public:
		void operator()(const T& elem){//operator 重载
			
			cout<<elem<<' ';
			
		}
};

int main(){
	int ia[6]={0,1,2,3,4,5};
	vector<int> iv(ia,ia+6);
	//print<int>()是函数调用操作，不是临时对象
	for_each(iv.begin(),iv.end(),print<int>());
	
	
}