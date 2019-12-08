ITEM26:��̳�������˫Ӥ����
	//�뿴�����������
	class BaseA
	{
	  virtual int ReadBuf( const char* );
	  // ...
	};
	
	class BaseB
	{
	  virtual int ReadBuf( const char* );
	  // ...
	};
	//BaseA��BaseB��һ����ͬ��---������Ȼ���뱻�������࣬���ǳ���֮������
	//������ɣ����ǵ�ReadBuf()������������ͬ�����飬�����������໹���Բ�ͬ����Ӧ��
	
	//ʾ�����дһ��Derived�࣬������BaseA��BaseB���м̳У����һ�Ҫ������ReadBuf()
	//���и�д������������ͬ������
	
	//�����д������ȷ
	// Example 26-1: Attempt #1, doesn't work
		//
		class Derived : public BaseA, public BaseB
		{
		  // ...
		
		  int ReadBuf( const char* );
		      // overrides both BaseA::ReadBuf()
		      // and BaseB::ReadBuf()
		};
		// Example 26-1(a): Counterexample,
		// why attempt #1 doesn't work
		//
		Derived d;
		BaseA*  pba = d;
		BaseB*  pbb = d;
		
		pba->ReadBuf( "sample buffer" );
		    // calls Derived::ReadBuf
		
		pbb->ReadBuf( "sample buffer" );
		    // calls Derived::ReadBuf

		//��ȷ��д��
		// Example 26-2: Attempt #2, correct
		//
		class BaseA2 : public BaseA
		{
		public:
		  virtual int BaseAReadBuf( const char* p ) = 0;
		private:
		  int ReadBuf( const char* p )    // override inherited
		  {
		    return BaseAReadBuf( p );     // to call new func
		  }
		};
		
		class BaseB2 : public BaseB
		{
		public:
		  virtual int BaseBReadBuf( const char* p ) = 0;
		private:
		  int ReadBuf( const char* p )   // override inherited
		  {
		    return BaseBReadBuf( p );    // to call new func
		  }
		};
		
		class Derived : public BaseA2, public BaseB2
		{
		  /* ... */
		
		public: // or "private:", depending whether other
		        // code should be able to call these directly
		
		  int BaseAReadBuf( const char* );
		      // overrides BaseA::ReadBuf indirectly
		      // via BaseA2::BaseAReadBuf
		
		  int BaseBReadBuf( const char* );
		      // overrides BaseB::ReadBuf indirectly
		      // via BaseB2::BaseBReadBuf
		};
		//����һ��������
		// Example 26-2(a): Why attempt #2 works
		//
		Derived d;
		BaseA*  pba = d;
		BaseB*  pbb = d;
		
		pba->ReadBuf( "sample buffer" );
		    // calls Derived::BaseAReadBuf
		
		pbb->ReadBuf( "sample buffer" );
		    // calls Derived::BaseBReadBuf
		
		
				
			
			
