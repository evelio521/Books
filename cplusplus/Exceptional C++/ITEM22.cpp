ITEM22:classes֮��Ĺ�ϵ
	//����Ĵ�������Ҫ�Ľ�����
	class BasicProtocol /* : possible base classes */ 
	{
	public:
	  BasicProtocol();
	  virtual ~BasicProtocol();//�������������������̳еģ�virtualû�б�Ҫ
	  bool BasicMsgA( /*...*/ );
	  bool BasicMsgB( /*...*/ );
	  bool BasicMsgC( /*...*/ );
	};
	
	class Protocol1 : public BasicProtocol
	{
	public:
	  Protocol1();
	  ~Protocol1();
	  bool DoMsg1( /*...*/ );
	  bool DoMsg2( /*...*/ );
	  bool DoMsg3( /*...*/ );
	  bool DoMsg4( /*...*/ );
	};
	
	class Protocol2 : public BasicProtocol
	{
	public:
	  Protocol2();
	  ~Protocol2();
	  bool DoMsg1( /*...*/ );
	  bool DoMsg2( /*...*/ );
	  bool DoMsg3( /*...*/ );
	  bool DoMsg4( /*...*/ );
	  bool DoMsg5( /*...*/ );
	};
	//ÿһ��DoMsg...() member function ������
	//BasicProtocol::Basic...()ִ�й�ͬ�Ĺ�����
	//Ȼ��DoMsg...()����ִ�д��͹�����
	
	//��������: ����Ҫʹ��public inheritance ��������Ҫ��Ĥ��������IS-A ��LIKE-A ��ϵ��
	        // ���б���д��member functions����Ҫ�����Ҳ���ܳ�ŵ���١�
	        
  //���׼��:����Ҫ��public inheritance����base class�ڵĳ�ʽ�룬public
          // inheritance��Ϊ�˱�����--����Щ�Զ��ͷ�ʽ����object�ĳ�ʽ�븴��
       1.BasicProtocolû���ṩ�κ����⺯������ʽ�������ζ������ϣ���Զ��͵ķ��Ǳ�ʹ��
         ���public inheritance��һ��ǿ�ҵķ��԰�ʾ
       2.BasicProtocolû���κ�protected members�������ζ��û�м̳нӿڣ�����κμ̳���ʽ
         (public ,private)����һ��ǿ�ҵķ��԰�ʾ
       3.BasicProtocol��װ�˹�ͬ�Ĺ���������������derived class����ִ���Լ��Ĵ��͹�����
         �����ζ��BasicProtocol�����������������һ��LIKE-A������protocol���
         Ҳ�ǿ���ΪIS-A ������protocol�����public inheritance Ӧ��ֻ��������ĤΨһһ������
         һ��������IS-A�ӿڹ�ϵ��
       4.����������classesֻ����BasicProtocol��public�ӿڡ������ζ������û����Ϊ�Լ���
         derived class�����棬���ǵĹ�������������һ���ͱ�ΪBasicProtocol�ĸ���������
    
    
    //���׼��: ������ϣ����Ĥ�� is implemented in terms of �Ĺ�ϵʱ����ѡ��membership/aggregation
               //����Ҫʹ��inheritance��ֻ���ھ��Ա�Ҫ�²�ʹ��private inheritance
               //Ҳ����˵������Ҫ��ȡprotected members������Ҫ��д���⺯��ʽʱ�����Բ�ҪΪ�˸��ó���ʽ�Ĵ���
               //ʹ��public inheritance
               
               
    