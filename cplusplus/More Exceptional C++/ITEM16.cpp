ITEM16:��ʽ�Ż�֮��:���̻߳���
	//Optimized::String�����ֳɰ�ȫ��
	void String::AboutToModify(
  size_t n,
  bool   markUnshareable /* = false */
  )
 {
  if( data_->refs > 1 && data_->refs != Unshareable )
  {//��������߳��У��߳�1��data_->refs > 1��ֵ���߳�2���ڸ���data_->refs >1
  	//��ֵ���ͻ������
    /* ... etc. ... */
   }
  }
  //�����������������ֱ���ʾ���ʹOptimized::String����̰߳�ȫ
    (a) ����߱���ȡ(get) ����(set) �ͱȽ�(compare)��ԭ�Ӳ���
    (b) ����û������ԭ�Ӳ���
   
    //���Ƚ��b��������������ʼǰ����Ҫ��Optimized::StringBuf��
    //����һ����Ա����������Ϊm
    namespace Optimized
		{
		
		  class StringBuf
		  {
		  public:
		      StringBuf();              // start off empty
		     ~StringBuf();              // delete the buffer
		      StringBuf( const StringBuf& other, size_t n = 0 );
		                                // initialize to copy of other,
		                                //  and ensure len >= n
		
		      void Reserve( size_t n ); // ensure len >= n
		
		      char*    buf;             // allocated buffer
		      size_t   len;             // length of buffer
		      size_t   used;            // # chars actually used
		      unsigned refs;            // reference count
		      Mutex    m;               // serialize work on this object
		
		  private:
		    // No copying...
		    //
		    StringBuf( const StringBuf& );
		    StringBuf& operator=( const StringBuf& );
		};
    //�ض���ͬʱ��������StringBuf����ĺ���ֻ��һ�������������캯����
    //Stringֻ���������ط�����StringBuf�Ŀ������캯��
    //(String����Ŀ������캯����AbouttoModify()) ��ע�⣬Stringֻ������ü���
    //���ʽ��д����Ż�����Ϊ���ݶ��壬û��String��Թ����StringBuf�����κβ���
    
    //ȱʡ�Ĺ��캯������Ҫ����
    String::String() : data_(new StringBuf) { }
		//��������ֻ���refs����ֵ�Ĳ�ѯ�͸��²�������
		String::~String()
		{
		  bool bDelete = false;
		  data_->m.Lock(); //---------------------------
		  if( data_->refs == Unshareable || --data_->refs < 1 )
		  {
		    bDelete = true;
		  }
		  data_->m.Unlock(); //-------------------------
		  if( bDelete )
		  {
		    delete data_;
		  }
		}
		//����String�Ŀ������캯�������ǿ��Լ��裬����������ڼ䣬������String
		//�����ݻ��������ᱻ�޸Ļ��ƶ�����Ϊ���Կɼ�����ķ��ʽ��д����Ż��ǵ����ߵ����Σ�
		//���Ƕ����ü���������ʣ����ǻ��ǵô��л�����
		String::String( const String& other )
		{
		  bool bSharedIt = false;
		  other.data_->m.Lock(); //---------------------
		  if( other.data_->refs != Unshareable )
		  {
		    bSharedIt = true;
		    data_ = other.data_;
		    ++data_->refs;
		  }
		  other.data_->m.Unlock(); //-------------------
		  if( !bSharedIt )
		  {
		    data_ = new StringBuf( *other.data_ );
		  }
		}
    
    //��������AboutToModify�������������ʵ���϶����������ϸ���˵
    //ֻ��Ҫ�������ط����������鿴refsֵ�Լ�������refsֵ�ĵط����������ǻ��Ƕ�������������
    void String::AboutToModify(
  		size_t n,
		  bool   markUnshareable /* = false */
			)
		{
		  data_->m.Lock(); //---------------------------
		  if( data_->refs > 1 && data_->refs != Unshareable )
		  {
		    StringBuf* newdata = new StringBuf( *data_, n );
		    --data_->refs;   // now all the real work is
		    data_->m.Unlock(); //-----------------------
		    data_ = newdata; //  done, so take ownership
		  }
		  else
		  {
		    data_->m.Unlock(); //-----------------------
		    data_->Reserve( n );
		  }
		  data_->refs = markUnshareable ? Unshareable : 1;
		}
		//������������Ҫ�޸�
		void String::Append( char c )
		{
		     AboutToModify( data_->used+1 );
		     data_->buf[data_->used++] = c;
		   }
		
		   size_t String::Length() const
		   {
		     return data_->used;
		   }
		
		   char& String::operator[]( size_t n )
		   {
		     AboutToModify( data_->len, true );
		     return data_->buf[n];
		   }
		
		   const char String::operator[]( size_t n ) const
		   {
		     return data_->buf[n];
		   }
		}
		//��������a�Ľ��
		//������һ���̰߳�ȫ��Stringʵ�֣���ʹ��������������
		//IntAtomicGet(), and IntAtomicDecrement() and IntAtomicIncrement()
		//�������������԰�ȫ�ķ�����ֵ�����Ǳ���������ǰ����ͬ�����飬
		//�����������ֻʹ��ԭ�Ӳ��������з���
		namespace Optimized
{


  String::String() : data_(new StringBuf) { }


  String::~String()
  {
    if( IntAtomicGet( data_->refs ) == Unshareable ||
        IntAtomicDecrement( data_->refs ) < 1 )
    {
      delete data_;
    }
  }


  String::String( const String& other )
  {
    if( IntAtomicGet( other.data_->refs ) != Unshareable )
    {
      data_ = other.data_;
      IntAtomicIncrement( data_->refs );
    }
    else
    {
      data_ = new StringBuf( *other.data_ );
    }
  }
  void String::AboutToModify(
        size_t n,
        bool   markUnshareable /* = false */
      )
      {
        int refs = IntAtomicGet( data_->refs );
        if( refs > 1 && refs != Unshareable )
        {
          StringBuf* newdata = new StringBuf( *data_, n );
          if( IntAtomicDecrement( data_->refs ) < 1 )
          {                  // just in case two threads
            delete newdata;  //  are trying this at once
          }
          else
          {                  // now all the real work is
            data_ = newdata; //  done, so take ownership
          }
        }
        else
        {
          data_->Reserve( n );
        }
        data_->refs = markUnshareable ? Unshareable : 1;
      }

      void String::Append( char c )
      {
        AboutToModify( data_->used+1 );
        data_->buf[data_->used++] = c;
      }

      size_t String::Length() const
      {
        return data_->used;
      }

      char& String::operator[]( size_t n )
      {
        AboutToModify( data_->len, true );
        return data_->buf[n];
      }

      const char String::operator[]( size_t n ) const
      {
        return data_->buf[n];
      }
    }
    //�������ϵ�Ӱ��
    //û��ԭ�Ӳ�����copy-on-write�����������ش��������ʧ��
    //��ʹ����ԭ������������COWҲ��ʹһ����ͨ��String����
    //��ʱ����50%�������ڵ��߳�Ҳ����ˡ�


		
		
		
		
		
