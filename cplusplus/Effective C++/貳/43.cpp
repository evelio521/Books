  /*ѧ�ᴦ��ģ�滯�����ڵ�����*/ 
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
  	
  };//����������Ϣ
  template<typename Company>
  class MsgSender{
  	
  	public:
  		void sendClear(const Info& info)
  		{
  			string msg;
  			//����info������Ϣ
  			Company c;
  			c.sendClearText(msg);
  		}
  		void sendSecret(const Info& info)
  		{
  			string msg;
  			//����info������Ϣ
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
  			//������ǰ����Ϣд��log
  			sendClear(info);//����base class���� �޷�ͨ������
  			//�����ͺ����Ϣд��log
  			
  		}
  };
  */
  /*
  //ģ��ȫ�ػ�
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
  class Info{ ....};//����������Ϣ
  template<typename Company>
  class MsgSender{
  	
  	public:
  		void sendClear(const Info& info)
  		{
  			string msg;
  			//����info������Ϣ
  			Company c;
  			c.sendClearText(msg);
  		}
  		void sendSecret(const Info& info)
  		{
  			string msg;
  			//����info������Ϣ
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
  			//����info������Ϣ
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
  			//������ǰ����Ϣд��log
  			sendClear(info);//���Company==CompanyZ �������������
  			//�����ͺ����Ϣд��log
  			
  		}
  };
  */
  //���������ְ취��C++ "������templatized base classes�۲�"����ΪʧЧ
    //��һ��
    template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		void sendClearMsg(const Info& info)
  		{
  			//������ǰ����Ϣд��log
  			this->sendClear(info);//���� ����sendClear�����̳�
  			//�����ͺ����Ϣд��log
  			
  		}
  };
  /*
  //�ڶ���
    template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		using MsgSender<Company>::sendClear;//���߱�����sendClearλ�ڻ���
  		void sendClearMsg(const Info& info)
  		{
  			//������ǰ����Ϣд��log
  			sendClear(info);//���� ����sendClear�����̳�
  			//�����ͺ����Ϣд��log
  			
  		}
  };
  //������
    template<typename Company>
  class LoggingMsgSender:public MsgSender<Company>{
  	
  	public:
  		
  		void sendClearMsg(const Info& info)
  		{
  			//������ǰ����Ϣд��log
  			 MsgSender<Company>::sendClear(info);//���� ����sendClear�����̳�
  			//�����ͺ����Ϣд��log
  			
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