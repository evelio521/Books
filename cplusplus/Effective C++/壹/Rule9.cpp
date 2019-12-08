/*绝不在构造函数和析构函数中调用virtual函数*/

//下面的做法是不好的
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
