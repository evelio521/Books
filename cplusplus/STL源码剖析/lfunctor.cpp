#include<iostream>
using namespace std;

//����operator() ��Ϊ�º���
template<class T>
struct plus{
	T operator()(const T&x,const T& y) const 
	{
		return x+y;
	}
};

//����operator() ��Ϊ�º���
template<class T>
struct minus{
	T operator()(const T& x,const T& y) const 
	{
		return x-y;
	}
};

int main(){
	//�����º�������
	plus<int> plusobj;
	minus<int> minusobj;
	
	cout<<plusobj(3,5)<<endl;
	cout<<minusobj(3,5)<<endl;
	
	cout<<plusobj<int>()(30,50)<<endl;
	cout<<minusobj<int>()(30,50)<<endl;
	
}