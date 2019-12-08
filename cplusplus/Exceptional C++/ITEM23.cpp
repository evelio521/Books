ITEM23:classes֮��Ĺ�ϵ
	//���Ͽ�Ӧ�����������Ҫ��ĳ����֪�ı���ڵ�ÿһ�ʼ�¼����һȺȦ�����ļ�¼��ĳЩ����
	//�����ǳ�����ִ��һЩΨ�������������������һ�飬�Ž���ȡװ���У��Ա��ȡ��Ҫ����ļ�¼
	//Ȼ�����ʵ�ʶ�����Щ����
	
	//��ʽԱ��ϣ��һ���ظ����ֳ������߼������������ṩһ�����ͳ�������ṩһ���ɸ��õĿ��
	//�뷨�ǳ�������ظ�������װ�������ռ���Ҫ�������������У������ִ�б�Ҫ������
	//����������ṩ�ض��Ķ���
	
	
	//--------------------------------------------------- 
	// File gta.h
	//---------------------------------------------------
	class GenericTableAlgorithm
	{
	public:
	  GenericTableAlgorithm( const string& table );
	  virtual ~GenericTableAlgorithm();
	
	  // Process() returns true if and only if successful.
	  // It does all the work: a) physically reads
	  // the table's records, calling Filter() on each
	  // to determine whether it should be included
	  // in the rows to be processed; and b) when the
	  // list of rows to operate upon is complete, calls
	  // ProcessRow() for each such row.
	  //
	  bool Process();
	
	private:
	  // Filter() returns true if and only if the row should be
	  // included in the ones to be processed. The
	  // default action is to return true (to include
	  // every row).
	  //
	  virtual bool Filter( const Record& );
	
	  // ProcessRow() is called once per record that
	  // was included for processing. This is where
	  // the concrete class does its specialized work.
	  // (Note: This means every row to be processed
	  // will be read twice, but assume that that is
	  // necessary and not an efficiency consideration.)
	  //
	  virtual bool ProcessRow( const PrimaryKey& ) =0;
	
	  struct GenericTableAlgorithmImpl* pimpl_; // MYOB
	};
	
	//For example, the client code to derive a concrete worker 
	//class and use it in a mainline looks something like this:
	
	class MyAlgorithm : public GenericTableAlgorithm 
	{
	  // ... override Filter() and ProcessRow() to
	  //     implement a specific operation ...
	};
	int main()
	{
	  MyAlgorithm a( "Customer" );
	  a.Process();
	}
	
	//���Ǹ��õ���� ʹ�����ĸ����ģʽ��Ϊʲô���ڴ˴����ã�
	
	  //����Template Method pattern����֮�������ã���Ϊ����ֻ��Ҫ��ѭ��ͬ�Ĳ��裬�Ϳ��Խ�ĳ�������ⷨһ�㻯��
	  //ֻ��ϸ�ڲ��ֲ�ͬ���˲��ֿ��������汾�ṩ��
	  
	  //���׼�򣺾�������ʹ��public���⺯��ʽ�������template method patternȡ��֮
	  //���׼���˽�ʲô��design pattern ������֮��
	  
	 //2.�ٲ��ı������ƵĻ����ϣ������۴�һ��Ƶ�ִ�з�ʽ������ʲô��ͬ��������pimpl_member��Ŀ�ģ�
	 
	   //��������bool��Ϊ�����룬��Ȼû������������¼ʧ�ܡ������������������Ǻõģ�
	   //������Щ������Ҫע�⡣������pimpl_member�ǳ�����Ľ�ʵ��ϸ��������һ����͸��ָ��֮��
	   //pimpl_ָ��Ľṹ���ں�private member functions��members variable
	   //ʹ�����ǵ��κθı䲻����ʹ��client���б�Ҫ������ơ�
	   
	//3. ����������ĸ���
	
	//���׼�򣺾����γ��ھۡ����Ǿ�����ÿһ�δ���---ÿһ��ģ�飬ÿһ�����ÿһ������ʽ--�ɵ�һ����ȷ������
	
	  //--------------------------------------------------- 
		// File gta.h
		//---------------------------------------------------
		// Responsibility #1: Providing a public interface
		// that encapsulates common functionality as a
		// template method. This has nothing to do with
		// inheritance relationships, and can be nicely
		// isolated to stand on its own in a better-focused
		// class. The target audience is external users of
		// GenericTableAlgorithm.
		//
		class GTAClient;
		
		class GenericTableAlgorithm
		{
		public:
		  // Constructor now takes a concrete implementation
		  // object.
		  //
		  GenericTableAlgorithm( const string& table,
		                         GTAClient&    worker );
		
		  // Since we've separated away the inheritance
		  // relationships, the destructor doesn't need to be
		  // virtual.
		  //
		  ~GenericTableAlgorithm();
		
		  bool Process(); // unchanged
		
		private:
		  struct GenericTableAlgorithmImpl* pimpl_; // MYOB
		};
		//---------------------------------------------------
		// File gtaclient.h
		//---------------------------------------------------
		// Responsibility #2: Providing an abstract interface
		// for extensibility. This is an implementation
		// detail of GenericTableAlgorithm that has nothing
		// to do with its external clients, and can be nicely
		// separated out into a better-focused abstract
		// protocol class. The target audience is writers of
		// concrete "implementation detail" classes which
		// work with (and extend) GenericTableAlgorithm.
		//
		class GTAClient
		{
		public:
		  virtual ~GTAClient() =0;
		  virtual bool Filter( const Record& );
		  virtual bool ProcessRow( const PrimaryKey& ) =0;
		};
		
		//---------------------------------------------------
		// File gtaclient.cpp
		//---------------------------------------------------
		bool GTAClient::Filter( const Record& )
		{
		  return true;
		}
		
		
		class MyWorker : public GTAClient 
		{
		  // ... override Filter() and ProcessRow() to
		  //     implement a specific operation ...
		};
		
		int main()
		{
		  GenericTableAlgorithm a( "Customer", MyWorker() );
		  a.Process();
		}
		//ע��������Ҫ������
		1.��� GenericTableAlgorithm�Ĺ�����������ı��ˣ���ԭ��������У����о����worker classes�������±���
		  ��Ϊ���������� GenericTableAlgorithm�����°汾�У� GenericTableAlgorithm����������κθı䶼������ĸ���
		  ��һ��Ҳ����Ӱ������worker classes
		2.��� GenericTableAlgorithm�Ŀ�����Э���ı��ˣ�ԭ���İ汾�У� GenericTableAlgorithm�������ⲿclient���������±���
		  ����Ϊ�ٴ�class�������ɼ����������档���������µİ汾�У� GenericTableAlgorithm�Ŀ�����Э�����ĸı䱻�����ˣ��κ�
		  �仯����Ӱ���ⲿʹ���ߡ�
		3.�κξ����worker classes�������������κ����㷨�б�ʹ��--ֻҪ�����㷨�ܹ�ʹ��Filter()��ProcessRow()����������--������
		  �� GenericTableAlgorithmʹ�ö��ѡ�
		  
		  
		  //���Խ�GenericTableAlgorithm��дΪ һ����ʽ�������Ǹ�classes
		  bool GenericTableAlgorithm( 
		  				const string& table,
		  				GTAClient&    method)
		{
		  // ... original contents of Process() go here ...
		}
		
		int main()
		{
		  GenericTableAlgorithm( "Customer", MyWorker() );
		}
		
		//�����ģ�沢���ã���ܾ���Ϊ���������д��һЩ�����ؼ�
		template<typename GTACworker> 
		bool GenericTableAlgorithm( const string& table )
		{
		  // ... original contents of Process() go here ...
		}
		
		int main()
		{
		  GenericTableAlgorithm<MyWorker>( "Customer" );
		}


		
		
				
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
