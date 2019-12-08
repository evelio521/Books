
class DBConnection{
	public:
		static DBConnection create();
		void close();
};
class DBConn{
	public:
		void close()
		{
			db.close();
			closed=true;
		}
		~DBConn()
		{
			if(!closed)
				{
					try{
						db.closed();
					}
					catch(...)
					{
						//制作运转记录，记下对close的调用失败
					}
				}
		}
	private:
		DBconnection db;
		bool closed;
};
//析构函数绝对不要吐出异常，如果一个析构函数吐出异常的话，
//析构函数应该捕获异常，吞下他们或结束程序
