ITEM14:缓式优化之二:引入缓式优化
	//有时候，在得到一个字符串对象拷贝后，用户可能不会在使用中做任何修改，然后
	//又丢弃了。对于一个String，我们每次都做了分配新缓冲区(开销很昂贵)，可是如
	//果所有用户只是从新字符串中读取数据然后摧毁。那么我所做的其实没有必要。我
	//们可以让两个字符串在底层共享一个缓冲区,暂时避免拷贝操作。只是在确实知道需
	//要拷贝的时候，也就是当其中一个对象试图修改这个字符串的时候，我才进行拷贝
	//如果用户永远不修改这个拷贝，我们就永远不做额外的工作。
	
	//下面是一个缓式拷贝的实现。对底层字符串实体实施引用计数
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
	  //实现 Optimized::StringBuf and Optimized::String，
	  //你可能需要辅助函数String::AboutToModify() 
	
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







