/*��operator=����һ��reference to *this*/

//ֻ��һ��Э�� ����ǿ�Ƶ�

class Widget{
	
	public:
		Widget& operator=(const Widget& rhs)
		{
			...
			return *this;
		}
		Widget& operator+=(const Widget& rhs)
		{
			...
			return *this;
		}
		Widget& operator=(int rhs)
		{
			...
			return *this;
		}
		
	
};