/*不要让异常逃离析构函数*/
 //具体的原因见教材
 
 //解决好的办法
 
  class DBConn{
  	
  	public:
  		...
  		void close()
  		{
  			db.close();
  			closed=true;
  		}
  	  ~DBConn(){
  	  	if(!closed)
  	  		{
  	  			try{
  	  				db.close();
  	  			}
  	  			catch(...){
  	  				制作运转记录，记下对close的调用失败;
  	  				...
  	  			}
  	  		}
  	  	
  	  }
  	  private:
  	  	DBconnection db;
  	  	bool closed;
  	
  };
