ITEM36:�ڴ����(��)
	//����ĳ�����һЩ��ʹ�������Լ����ڴ���������ҳ�����
	
			class B 
		{
			//ΪʲôB��delete���еڶ���������
	    //ΪʲôD��deleteû�еڶ���������
	    //�����ֶ����������Ļ��գ������ϲ�ò�ͬ
	    //����û���ṩ��Ӧ��new ��new[]�Ƿǳ�Σ�յ�
		public:
		  virtual ~B();
		  void operator delete  ( void*, size_t ) throw();
		  void operator delete[]( void*, size_t ) throw();
		  void f( void*, size_t ) throw();
		};
		class D : public B
		{
		public:
		  void operator delete  ( void* ) throw();
		  void operator delete[]( void* ) throw();
		};
		
    void f()
    {
    	//��������䣬������һ��delete�������ˣ�Ϊʲô��
    	//����ʱ�Ĳ�����ʲô
    	D* pd1 = new D; 
			delete pd1;//D::operator delete(void*).
			B* pb1 = new D;
			delete pb1;//D::operator delete(void*). 
			D* pd2 = new D[10];
			delete[] pd2;//D::operator delete[](void*).
			B* pb2 = new D[10];
			delete[] pb2;//����Ԥ�ϵģ�C++����Ҫ�󣬴��ݸ�delete��ָ��ľ�̬���ͱ���Ͷ�̬����һ����


    }
    //����������ֵ���Ϸ���
    B b;
    typedef void (B::*PMF)(void*, size_t); 
		PMF p1 = &B::f;
		PMF p2 = &B::operator delete;
		//��һ����ֵ���Ϸ�
		//�ڶ������Ϸ�����Ϊ void operator delete( void*, size_t ) throw() 
		//������B�ĳ�Ա��������Ϊnew��delete�Ǿ�̬��
		
		//С����
		//new��delete���Ǿ�̬�ģ���ʹ����ʾ������Ϊstatic�����ǰ�����
		//����Ϊstatic�Ǹ��õ�ϰ�ߣ����԰���������
		class X 
		{
		public:
		  void* operator new( size_t s, int )
		                   throw( bad_alloc )
		  //������ڴ�й©����Ϊû����Ӧ��placement delete���ڡ�
		  {
		    return ::operator new( s );
		  }
		};
		class SharedMemory
		{
		public:
		  static void* Allocate( size_t s )
		  {
		    return OsSpecificSharedMemAllocation( s );
		  }
		    static void  Deallocate( void* p, int i = 0 )
		    {
		      OsSpecificSharedMemDeallocation( p, i );
		    }
		  };
		  
		  class Y
		  {
		  public:
		    void* operator new( size_t s,
		                        SharedMemory& m ) throw( bad_alloc )
		    {
		      return m.Allocate( s );
		    }
		    //��û��operator deleteҲ������ڴ�й©�����ڹ��캯���׳��쳣�Ͳ����ͷ��ڴ�
		   /* void  operator delete( void* p,
		                           SharedMemory& m,
		                           int i ) throw()
		    {
		       m.Deallocate( p, i );
		    }*/
		  };
		  //������Ĺ��캯���׳��쳣
		  SharedMemory shared; 
			...
			new (shared) Y; // if Y::Y() throws, memory is leaked
			
			
			void operator delete( void* p, 
                        SharedMemory& m,
                        int i ) throw()
			  {
			  	//�����delete�����ô�����Ϊ�����ᱻ����
			    m.Deallocate( p, i );
			  }
			};
			void operator delete( void* p, 
                      std::nothrow_t& ) throw()
			{
				//����һ�����صĴ�����Ϊ����Ҫɾ����Щȱʡ::operator new����������ڴ�
				//������shareMemory::Allocate()����������ڴ档
			  SharedMemory::Deallocate( p );
			}
			
			void operator delete( void* p, 
                      std::nothrow_t& ) throw()
			{//��Ҳ��һ����ֻ�Ǹ�Ϊ΢�����deleteֻ����new(nothrow) Tʧ�ܵ�ʱ��Żᱻ����
			 //��ΪT�Ĺ��캯�������һ���쳣����ֹ������ͼ������Щ���Ǳ�SharedMemory::Allocate����������ڴ�
			 SharedMemory::Deallocate( p );
			}


			
			      
