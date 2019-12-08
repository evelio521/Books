/*令operator=返回一个reference to *this*/

//只是一个协议 不是强制的

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