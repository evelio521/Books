/*��Ҫ���쳣������������*/
 //�����ԭ����̲�
 
 //����õİ취
 
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
  	  				������ת��¼�����¶�close�ĵ���ʧ��;
  	  				...
  	  			}
  	  		}
  	  	
  	  }
  	  private:
  	  	DBconnection db;
  	  	bool closed;
  	
  };
