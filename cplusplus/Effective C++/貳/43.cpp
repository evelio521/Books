  /*学会处理模版化基类内的名称*/ 
  #include <string>
  #include <iostream>
using namespace std;
  class CompanyA{
  	public:
		void sendClearText(const string& msg)
		{
		}
		void sendEncryptedText(const string& msg)
		{
		}
  		
  };
  class CompanyB{
  	public:
  		void sendClearText(const string& msg){}
  		void sendEncryptedText(const string& msg){}
  		
  };
  class Info{ 
  	
  };//用来保存信息
  template<typename Company>
  class MsgSender{
  	
  	public:
  		void sendClear(const Info& info)
  		{
  			string msg;
  			//根据info产生信息
  			Company c;
  			c.sendClearText(msg);
  		}
  		void sendSecret(const Info& info)
  		{
  			string msg;
  			//根据info产生信息
  			Company c;
  			c.sendEncryptedText(msg);
  		}
  	private:
  		//string msg;
  		//Company c;
  		
  };
  /*
  template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		void sendClearMsg(const Info& info)
  		{
  			//将传送前的信息写入log
  			sendClear(info);//调用base class函数 无法通过编译
  			//将传送后的信息写入log
  			
  		}
  };
  */
  /*
  //模版全特化
  class CompanyA{
  	public:
  		void sendClearText(const string& msg);
  		void sendEncryptedText(const string& msg);
  		
  };
  class CompanyB{
  	public:
  		void sendClearText(const string& msg);
  		void sendEncryptedText(const string& msg);
  		
  };
  class Info{ ....};//用来保存信息
  template<typename Company>
  class MsgSender{
  	
  	public:
  		void sendClear(const Info& info)
  		{
  			string msg;
  			//根据info产生信息
  			Company c;
  			c.sendClearText(msg);
  		}
  		void sendSecret(const Info& info)
  		{
  			string msg;
  			//根据info产生信息
  			Company c;
  			c.sendClearText(msg);
  		}
  };*/
  class CompanyZ{
  	public:
  		void sendEncryptedText(const string& msg)
  		{
  		}
  		
  };
  
  template<>
  class MsgSender<CompanyZ>{
  	
  	public:
  		
  		void sendSecret(const Info& info)
  		{
  			string msg;
  			//根据info产生信息
  			CompanyZ c;
  			c.sendEncryptedText(msg);
  		}
  };
  /*
  template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		void sendClearMsg(const Info& info)
  		{
  			//将传送前的信息写入log
  			sendClear(info);//如果Company==CompanyZ 这个函数不存在
  			//将传送后的信息写入log
  			
  		}
  };
  */
  //我们有三种办法苓C++ "不进入templatized base classes观察"的行为失效
    //第一种
    template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		void sendClearMsg(const Info& info)
  		{
  			//将传送前的信息写入log
  			this->sendClear(info);//成立 假设sendClear将被继承
  			//将传送后的信息写入log
  			
  		}
  };
  /*
  //第二种
    template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		using MsgSender<Company>::sendClear;//告诉编译器sendClear位于基类
  		void sendClearMsg(const Info& info)
  		{
  			//将传送前的信息写入log
  			sendClear(info);//成立 假设sendClear将被继承
  			//将传送后的信息写入log
  			
  		}
  };
  //第三种
    template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		
  		void sendClearMsg(const Info& info)
  		{
  			//将传送前的信息写入log
  			 MsgSender<Company>::sendClear(info);//成立 假设sendClear将被继承
  			//将传送后的信息写入log
  			
  		}
  };*/
  int main(){
  	
  	MsgSender<CompanyA> ms;
	const Info info;
	ms.sendClear(info);
	ms.sendSecret(info);
  	LoggingMsgSender<CompanyA> lm;
	lm.sendClear(info);
	lm.sendClearMsg(info);
  	
  }