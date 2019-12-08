//确保你的构造函数和析构函数中没有virtual函数 否则会引发灾难
//因为这类调用不会下降至 derived class

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

	BuyTransaction b;//后果见教材
	
	//解决方法
	
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