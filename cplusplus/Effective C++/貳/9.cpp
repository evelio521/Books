//ȷ����Ĺ��캯��������������û��virtual���� �������������
//��Ϊ������ò����½��� derived class

class  Transaction{
	
	public:
		Transaction();
		virtual void logTransaction()const=0;
};

Transaction::Transaction()
	{
		...
		logTransaction();
	}
	
class BuyTransaction:public Transaction{
	
	public:
		virtual void logTransaction() const;
	
};
class SellTransaction:public Transaction{
	
	public:
		virtual void logTransaction() const;
	
};

	BuyTransaction b;//������̲�
	
	//�������
	
	class  Transaction{
	
	public:
		explicit Transaction(const std::string &logInfo);
		void logTransaction(const std::string &logInfo)const;
};

Transaction::Transaction(const std::string &logInfo)
	{
		...
		logTransaction(logInfo);
	}
	
class BuyTransaction:public Transaction{
	
	public:
		BuyTransaction(parameters):Transaction(createLogString(parameters))
			{
			}
		//virtual void logTransaction() const;
	private:
		static std:string createLogString(parameters);
	
};