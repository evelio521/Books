/*���ƶ���ʱ������ÿһ���ɷ�*/

/*
 *  ���Լ�����copy �� copy assign������ʱ�򣬲�Ҫ��������ÿһ��
 *  ��Ա��������ʹ�Ǽ̳����ڶ���copy �� copy assign������ʱ��
 *  ҲҪ��ʾ�ĶԻ���ĳ�Ա������copy �� copy assign�����Ľ���
 *  ��ʾ�ĳ�ʼ����
 *  ����Ĵ�����̲�
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