#include<iostream>

//编写程序时候 应该尽量让借口变得容易使用 而不是被误用
struct Day{
	explicit Day(int d):val(d){
	
	}
	int val;
};
class Month{
	public:
		static Month Jau(){return Month(1);}
		static Month Feb(){return Month(2);}
		static Month Mar(){return Month(3);}
		static Month Apr(){return Month(4);}
		static Month May(){return Month(5);}
		static Month Jun(){return Month(6);}
		static Month Jul(){return Month(7);}
		static Month Aug(){return Month(8);}
		static Month Sep(){return Month(9);}
		static Month Oct(){return Month(10);}
		static Month Nov(){return Month(11);}
		static Month Dec(){return Month(12);}
	private:
	explicit Month(int d):val(d){
	
	}
	int val;
};
struct Year{
	explicit Year(int d):val(d){
	
	}
	int val;
};

class Date{
	public:
		Date(const Month& m,const Day& d,const Year& y):month(m),day(d),year(y){
			
		}
	private:
		Month month;
		Day day;
		Year year;
		
	
};

int main(){

	Date d(Month::Feb(),Day(28),Year(2012));
	
	
	
};