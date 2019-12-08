ITEM15:��ʽ�Ż�֮��:������������
	//����copy-on-write������Optimized::String�࣬������Length(),operator[]()��������
	namespace Optimized
	{
	  class StringBuf
	  {
	  public:
	    StringBuf();              // start off empty
	   ~StringBuf();              // delete the buffer
	    void Reserve( size_t n ); // ensure len >= n
	
	    char*    buf;             // allocated buffer
	    size_t   len;             // length of buffer
	    size_t   used;            // # chars actually used
	    unsigned refs;            // reference count
	
	  private:
	    // No copying...
	    //
	    StringBuf( const StringBuf& );
	    StringBuf& operator=( const StringBuf& );
	  };
	
	  class String
	  {
	  public:
	    String();                 // start off empty
	     ~String();                 // decrement reference count
	                                //  (delete buffer if refs==0)
	      String( const String& );  // point at same buffer and
	                                //  increment reference count
	      void   Append( char );    // append one character
	
	      size_t Length() const;    // string length
	
	      char&  operator[](size_t);// element access
	      const char operator[](size_t) const;
	
	      // ... operator=() etc. omitted ...
	    private:
	      void AboutToModify( size_t n );
	                                // lazy copy, ensure len>=n
	      StringBuf* data_;
	    };
	  }
	
	//This allows code such as the following:
	
	if( s.Length() > 0 )
	{
	  cout << s[0];
	  s[0] = 'a';
	}

  //ʵ��Optimized::String�³�Ա�����Ӻ�����������ʲô��Ա��Ҫ�޸���
  
  //ʵ��Optimized::String�³�Ա
  
  namespace Optimized
	{
	
	  size_t String::Length() const
	  {
	    return data_->used;
	  }
	 }
	 //����operator[]()�Ƚ��鷳�������ǱȽ����ɵ�����
	 // BAD: Na?ve attempt #1 at operator[]
		//
		char& String::operator[]( size_t n )
		{
		  return data_->buf[n];
		}
		
		const char String::operator[]( size_t n ) const
		{
		  return data_->buf[n];
		}
		//���������Ĳ���
		// Example 15-1: Why attempt #1 doesn't work
		//
		void f( const Optimized::String& s )
		{
		  Optimized::String s2( s ); // take a copy of the string
		  s2[0] = 'x';               // oops: also modifies s!
		}
		
		//����operator[]()�Ƚ��鷳�������ǱȽ����ɵ�����
		// BAD: Inadequate attempt #2 at operator[]
		//
		char& String::operator[]( size_t n )
		{
		  AboutToModify( data_->len );
		  return data_->buf[n];
		}
		
		const char String::operator[]( size_t n ) const
		{
		  // no need to check sharing status this time
		  return data_->buf[n];
		}
		//���������Ĳ���
		// Example 15-2: Why attempt #2 doesn't work either
		//
		void f( Optimized::String& s )
		{
		  char& rc = s[0];  // take a reference to the first char
		  Optimized::String s2( s ); // take a copy of the string
		  rc = 'x';                  // oops: also modifies s2!
		}
		//����Ҫ���ľ��ǲ�����String�����ǿ��Խ�String���Ϊ��Զ������Ҳ���Ա��Ϊ
		//��ʱ���������operator[]����һ��ָ���ַ����ڲ������ã���ô��һ���޸ĺ�
		//���Ǳ������Ǹ�������Ч������:
		// Example 15-3: Why references are
		// invalidated by mutating operations
		//
		void f( Optimized::String& s )
		{
		  char& rc = s[0];
		  s.Append( 'i' );
		  rc = 'x';   // ����:���sִ�������·���
		}             // ���������ܱ��ƶ�
		
		//�������³�Ա����Щ��Ա����Ҫ�޸ĵ�
		//����operator[]��ȷ�ķ���
		//Ϊ�˷��㣬����һ���¾�̬��Ա
		//����AboutToModify()���ʵ��޸�
		//��Ϊ������Ҫ�ڲ�ֹһ�������и���StringBuf
		//���ǻ���Ҫ����һ�߼��Ž�һ������������
		//��������Stringbuf���ڵ���һ���Լ��Ŀ������캯��
		const size_t String::Unshareable =numeric_limits<size_t>::max();
		StringBuf::StringBuf( const StringBuf& other, size_t n )
		  : buf(0), len(0), used(0), refs(1)
		{
		  Reserve( max( other.len, n ) );
		  copy( other.buf, other.buf+other.used, buf );
		  used = other.used;
		}
		
		void String::AboutToModify(
		  size_t n,
		  bool   markUnshareable /* = false */
		)
		{
		  if( data_->refs > 1 && data_->refs != Unshareable )
		  {
		    StringBuf* newdata = new StringBuf( *data_, n );
		    --data_->refs;   // now all the real work is
		    data_ = newdata; //  done, so take ownership
		  }
		  else
		  {
		    data_->Reserve( n );
		  }
		  data_->refs = markUnshareable ? Unshareable : 1;
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
		
		//��������˲��ɹ����״ֵ̬�����ǻ�Ҫ��String�Ŀ������캯��
		//��ʹ����
		String::String( const String& other )
		{
		  // If possible, use copy-on-write.
		  // Otherwise, take a deep copy immediately.
		  //
		  if( other.data_->refs != Unshareable )
		  {
		    data_ = other.data_;
		    ++data_->refs;
		  }
		  else
		  {
		    data_ = new StringBuf( *other.data_ );
		  }
		}
		//String����������ҲСС�Ķ�
		String::~String()
		{
		  if( data_->refs == Unshareable || --data_->refs < 1 )
		  {
		    delete data_;
		  }
		}
		//String�������������������д������
		String::String() : data_(new StringBuf) { }

	  void String::Append( char c )
	  {
	    AboutToModify( data_->used+1 );
	    data_->buf[data_->used++] = c;
	  }
	
	}
	//��������:��StringBuf::Reserve(); ��СС���޸ģ�������»�������С�������������
	//��ʹ����ֵ��������һ��4�ı������Ӷ���֤�ڴ滺�����Ĵ�С����4�ֽڵı���������ΪЧ��
	namespace Optimized {

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

    private:
      // No copying...
      //
      StringBuf( const StringBuf& );
      StringBuf& operator=( const StringBuf& );
    };

    class String
    {
    public:
      String();                 // start off empty
     ~String();                 // decrement reference count
                                //  (delete buffer if refs==0)
      String( const String& );  // point at same buffer and
                                //  increment reference count
      void   Append( char );    // append one character
      size_t Length() const;    // string length
      char&  operator[](size_t);// element access
      const char operator[](size_t) const;

      // ... operator=() etc. omitted ...

    private:
      void AboutToModify( size_t n, bool bUnshareable = false );
                                // lazy copy, ensure len>=n
                                //  and mark if unshareable
      static size_t Unshareable;// ref-count flag for "unshareable"
      StringBuf* data_;
    };

    StringBuf::StringBuf()
      : buf(0), len(0), used(0), refs(1) { }

    StringBuf::~StringBuf() { delete[] buf; }

    StringBuf::StringBuf( const StringBuf& other, size_t n )
      : buf(0), len(0), used(0), refs(1)
    {
        Reserve( max( other.len, n ) );
        copy( other.buf, other.buf+other.used, buf );
        used = other.used;
    }
		void StringBuf::Reserve( size_t n )
		{
		  if( len < n )
		  {
		    // Same growth code as in Item 14, except now we round
		    // the new size up to the nearest multiple of 4 bytes.
		    size_t needed = max<size_t>( len*1.5, n );
		    size_t newlen = needed ? 4 * ((needed-1)/4 + 1) : 0;
		    char*  newbuf = newlen ? new char[ newlen ] : 0;
		    if( buf )
		    {
		      copy( buf, buf+used, newbuf );
		    }
		
		    delete[] buf;   // now all the real work is
		    buf = newbuf;   //  done, so take ownership
		    len = newlen;
		  }
		}
		
		
		const size_t String::Unshareable = numeric_limits<size_t>::max();
		
		String::String() : data_(new StringBuf) { }
		
		String::~String()
		{
		  if( data_->refs == Unshareable || --data_->refs < 1 )
		  {
		    delete data_;
		  }
		}
		
		String::String( const String& other )
		{
		  // If possible, use copy-on-write.
		  // Otherwise, take a deep copy immediately.
		  //
		  if( other.data_->refs != Unshareable )
		  {
		    data_ = other.data_;
		    ++data_->refs;
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
		      if( data_->refs > 1 && data_->refs != Unshareable )
		      {
		        StringBuf* newdata = new StringBuf( *data_, n );
		        --data_->refs;   // now all the real work is
		        data_ = newdata; //  done, so take ownership
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
		
		
		
		
				
				
						
						
						
