ITEM14:��ʽ�Ż�֮��:���뻺ʽ�Ż�
	//��ʱ���ڵõ�һ���ַ������󿽱����û����ܲ�����ʹ�������κ��޸ģ�Ȼ��
	//�ֶ����ˡ�����һ��String������ÿ�ζ����˷����»�����(�����ܰ���)��������
	//�������û�ֻ�Ǵ����ַ����ж�ȡ����Ȼ��ݻ١���ô����������ʵû�б�Ҫ����
	//�ǿ����������ַ����ڵײ㹲��һ��������,��ʱ���⿽��������ֻ����ȷʵ֪����
	//Ҫ������ʱ��Ҳ���ǵ�����һ��������ͼ�޸�����ַ�����ʱ���ҲŽ��п���
	//����û���Զ���޸�������������Ǿ���Զ��������Ĺ�����
	
	//������һ����ʽ������ʵ�֡��Եײ��ַ���ʵ��ʵʩ���ü���
		namespace Optimized
	{
	  class StringBuf
	  {
	  public:
	    StringBuf();             // start off empty
	   ~StringBuf();             // delete the buffer
	    void Reserve( size_t n );// ensure len >= n
	
	    char*    buf;            // allocated buffer
	    size_t   len;            // length of buffer
	    size_t   used;           // # chars actually used
	    unsigned refs;           // reference count
	
	  private:
	    // No copying...
	    //
	    StringBuf( const StringBuf& );
	    StringBuf& operator=( const StringBuf& );
	  };
	
	  class String
	  {
	  public:
	    String();                // start off empty
	   ~String();                // decrement reference count
	                             //  (delete buffer if refs==0)
	    String( const String& ); // point at same buffer and
	                             //  increment reference count
	    void   Append( char );   // append one character
	
	    // ... operator=() etc. omitted ...
	
	  private:
	    StringBuf* data_;
	    };
	  }
	  //ʵ�� Optimized::StringBuf and Optimized::String��
	  //�������Ҫ��������String::AboutToModify() 
	
		namespace Optimized
		{
		
		  StringBuf::StringBuf()
		      : buf(0), len(0), used(0), refs(1) { }
		    StringBuf::~StringBuf() { delete[] buf; }
		void StringBuf::Reserve( size_t n )
		{
		  if( len < n )
		  {
		    size_t newlen = max( len * 1.5, n );
		    char*  newbuf = new char[ newlen ];
		    copy( buf, buf+used, newbuf );
		
		    delete[] buf;   // now all the real work is
		    buf = newbuf;   //  done, so take ownership
		    len = newlen;
		  }
		}
		String::String() : data_(new StringBuf) { }
		String::~String()
		{
		  if( --data_->refs < 1 )  // last one out ...
		  {
		    delete data_;  // ... turns off the lights
		  }
		}
		String::String( const String& other )
		  : data_(other.data_)
		{
		  ++data_->refs;
		}
		void String::AboutToModify( size_t n )
		{
		  if( data_->refs > 1 )
		  {
		    auto_ptr<StringBuf> newdata( new StringBuf );
		    newdata->Reserve( max( data_->len, n ) );
		    copy( data_->buf, data_->buf+data_->used, newdata->buf );
		    newdata->used = data_->used;
		
		    --data_->refs;             // now all the real work is
		    data_ = newdata.release(); //  done, so take ownership
		  }
		  else
		  {
		    data_->Reserve( n );
		  }
		}
		void String::Append( char c ) {
		    AboutToModify( data_->used+1 );
		    data_->buf[used++">data_->used++] = c;
		  }
		
		}







