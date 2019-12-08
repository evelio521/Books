#include<iostream>
using namespace std;

//重载operator() 成为仿函数
template<class T>
struct plus{
	T operator()(const T&x,const T& y) const 
	{
		return x+y;
	}
};

//重载operator() 成为仿函数
template<class T>
struct minus{
	T operator()(const T& x,const T& y) const 
	{
		return x-y;
	}
};

int main(){
	//产生仿函数对象
	plus<int> plusobj;
	minus<int> minusobj;
	
	cout<<plusobj(3,5)<<endl;
	cout<<minusobj(3,5)<<endl;
	
	cout<<plusobj<int>()(30,50)<<endl;
	cout<<minusobj<int>()(30,50)<<endl;
	
}