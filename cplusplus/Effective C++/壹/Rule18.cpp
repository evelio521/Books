/*�ý����Ƶĸ�����ʹ��*/

//���һ��Date�� ��Ҳ����������

class Date{
	public:
		Date(int month,int day,int year);
		...
};

//Ҳ���������ƻ���õ�
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

//һ��ֻ��12���� ��ö�������е��·�
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