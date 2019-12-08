ITEM16:缓式优化之四:多线程环境
	//Optimized::String不是现成安全的
	void String::AboutToModify(
  size_t n,
  bool   markUnshareable /* = false */
  )
 {
  if( data_->refs > 1 && data_->refs != Unshareable )
  {//如果两个线程中，线程1对data_->refs > 1求值而线程2正在更改data_->refs >1
  	//的值，就会出问题
    /* ... etc. ... */
   }
  }
  //针对下面两种情况，分别演示如何使Optimized::String变得线程安全
    (a) 假设具备获取(get) 设置(set) 和比较(compare)的原子操作
    (b) 假设没有以上原子操作
   
    //首先解决b，在其他工作开始前，需要在Optimized::StringBuf中
    //增加一个成员对象，其名称为m
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
    //必定会同时操作两个StringBuf对象的函数只有一个，即拷贝构造函数，
    //String只会在两个地方调用StringBuf的拷贝构造函数
    //(String自身的拷贝构造函数和AbouttoModify()) ，注意，String只需对引用计数
    //访问进行串行优化，因为根据定义，没有String会对共享的StringBuf进行任何操作
    
    //缺省的构造函数不需要加锁
    String::String() : data_(new StringBuf) { }
		//析构函数只需对refs计数值的查询和更新操作加锁
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
		//对于String的拷贝构造函数，我们可以假设，在这个操作期间，其他的String
		//的数据缓冲区不会被修改或移动，因为，对可见对象的访问进行串行优化是调用者的责任，
		//但是对引用计数本身访问，我们还是得串行化处理
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
    
    //再来看看AboutToModify，这里深拷贝操作实际上都被加锁。严格来说
    //只需要在两个地方加锁，即查看refs值以及最后更新refs值的地方。但是我们还是对整个操作加锁
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
		//其他函数不需要修改
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
		//对于问题a的解决
		//这里是一个线程安全的String实现，它使用了三个函数，
		//IntAtomicGet(), and IntAtomicDecrement() and IntAtomicIncrement()
		//这三个函数可以安全的返回新值，我们本质是做与前面相同的事情，
		//但是这里，我们只使用原子操作来串行访问
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
    //在性能上的影响
    //没有原子操作，copy-on-write往往会招致重大的性能损失，
    //即使有了原子整数操作，COW也会使一个普通的String操作
    //耗时增长50%，甚至在单线程也是如此。


		
		
		
		
		
