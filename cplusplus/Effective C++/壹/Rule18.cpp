/*让借口设计的更容易使用*/

//设计一个Date类 你也许会这样设计

class Date{
	public:
		Date(int month,int day,int year);
		...
};

//也许下面的设计会更好点
 struct Day{                   struct Month{                       struct Year{
 	
 	explicit Day(int d):val(d)       	explicit Month(int m):val(m)        explicit Year(int y):val(y)
 		{                                   {                                    {
 	}                                      }                                    }
 	int val;                          	int val;                           int val;
};                                   };                                };
 class Date{
	public:
		Date(const Month& m,const Day& day,const Year& year);
		...
};

//一年只有12个月 最好定义好所有的月份
class Month{
	public:
		static Month Jan() { return Month(1);} 
		static Month Feb() { return Month(2);} 
		...
		static Month Dec() { return Month(12);} 
	private:
		explicit Month(int m);
		...
};