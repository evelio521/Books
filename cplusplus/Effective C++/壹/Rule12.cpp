/*复制对象时勿忘其每一个成分*/

/*
 *  当自己定义copy 和 copy assign函数的时候，不要忽略类中每一个
 *  成员变量，即使是继承类在定义copy 和 copy assign函数的时候
 *  也要显示的对基类的成员变量的copy 和 copy assign函数的进行
 *  显示的初始化。
 *  具体的代码见教材
 */
 
 class Cuetomer{
 	public:
 		...
 		Customer(const Customer& rhs);
 		Cuetomer& operator=(const Customer& rhs);
 		...
 	private:
 		string name;
};
Customer::Customer(const Customer& rhs):name(rhs.name)
	{
		
	}
	Customer& Customer::operator=(const Customer& rhs)
	{
		name=rhs.name;
		return *this;
		
	}
	
	class PriorotyCustomer:public Customer{
		public:
			...
			PriorotyCustomer(const PriorotyCustomer& rhs);
 			PriorotyCustomer& operator=(const PriorotyCustomer& rhs);
 		private:
 			int priority;
	};
	
	PriorotyCustomer::PriorotyCustomer(const PriorotyCustomer& rhs):Customer(rhs),priority(rhs.priority)
	{
		
	}
	PriorotyCustomer& PriorotyCustomer::operator=(const PriorotyCustomer& rhs)
	{
		Customer::operator=(rhs);
		priority=rhs.priority;
		return *this;
		
	}