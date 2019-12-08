#include<new>
#include<iostream>
#include<string>
using namespace std;

class Widget{
	
	public:
		Widget(int wid):val(wid){
			
		}
private:
	int val;
};

int main(){
	
	string *ps=new string("string");
		
	void *rawMemory=operator new(10*sizeof(Widget));
		
	int val=23;
	
	Widget *widget=new(rawMemory) Widget(val);;
	
	string *str=new string[10];

	delete [] str;

	//delete widget;
	operator delete(rawMemory);
	delete ps;
	
}