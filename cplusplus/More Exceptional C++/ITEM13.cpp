ITEM13:缓式优化之一:一个普通的旧式String
	//分析下面简式的String类
		namespace Original
	{
	  class String
	  {
	  public:
	    String();                // start off empty
	   ~String();                // free the buffer
	    String( const String& ); // take a full copy
	    void Append( char );     // append one character
	
	    // ... operator=() etc. omitted ...
	
	  private:
	    char*    buf_;           // allocated buffer
	    size_t   len_;           // length of buffer
	    size_t   used_;          // # chars actually used
	  };
	  }
	//对上面的实现
	namespace Original {

  String::String() : buf_(0), len_(0), used_(0) { }

  String::~String() { delete[] buf_; }
  
  String::String( const String& other )
         : buf_(new char[other.len_]),
           len_(other.len_),
           used_(other.used_)
   {
   copy( other.buf_, other.buf_ + used_, buf_ );
   }
		void String::Reserve( size_t n )
		{
		  if( len_ < n )
		  {
		    size_t newlen = max( len_ * 1.5, n );
		    char*  newbuf = new char[ newlen ];
		    copy( buf_, buf_+used_, newbuf );
		
		    delete[] buf_;  // now all the real work is
		    buf_ = newbuf;  //  done, so take ownership
		    len_ = newlen;
		  }
		}
		 void String::Append( char c )
	  {
	    Reserve( used_+1 );
	    buf_[used_++] = c;
	  }

   
  }


  //缓冲区增长策略
  1.精确增长
  2.固定增量增长
  3.指数增长
  
  增长策略     Allocations       Char Copies    Wasted Space  
   精确增长     O(N)高常数        O(N)高常数        无  
  固定增量增长    O(N)              O(N)           O(1)  
   指数增长       O(logN)           O(1)           O(N)  
