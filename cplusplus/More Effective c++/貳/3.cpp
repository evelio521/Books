#include<iostream>
using namespace std;
class Base{
	public:
		Base():ivale(0){}
		friend ostream& operator<<(ostream& s,const Base& b);
		
	private:
		int ivale;
	
};
ostream& operator<<(ostream& s,const Base& b){
			
			return s<<b.ivale;


		}
class Derived:public Base{
	
	public:
		Derived():Base(),ival(1){}
		
		
	private:
		int ival;
		
};
void printBD(ostream& s,const Base array[],int length)
{
	for(int i=0;i<length;i++)
		s<<array[i];
}
//ͨ��baseָ��ɾ����derived object��ɵ���������δ�����
//����ĺ�ʽ������δ����Ľ��
void deleteBD(ostream& s,const Base array[])
{
	delete [] array;
}
int main(){
	
	Base b[10];
	printBD(cout,b,10);
	Derived d[10];
	printBD(cout,d,10);

	Derived *dd=new Derived[80];
	deleteBD(cout,dd);
	
}