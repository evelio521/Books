ITEM8-17:异常的安全性
	//实现如下异常--中立的Container。要求：stack对象的状态必须保持一致性
	//即有内部异常抛出，stack对象必须是可析构的：T的异常可以传到调用者那里
	template <class T>
	class Stack{
		public:
			Stack();
			~Stack()
			Stack(const Stack&);
			Stack& operator=(const Stack&);
			unsigned Count();//返回栈内的元素数目
			void Push(const T&);//入栈
			T pop();//出栈
		private:
			T* v_;//指向一个用于vsize_ T对象的足够大的空间
			unsigned vsize_;//v_区域的大小
			unsigned vused_;//v_区域实际使用
			
	};
	
	//以下是实现
	
	template<class T> 
Stack<T>::Stack()
  : v_(new T[10]),  // default allocation  //有内部异常抛出时，T可以处理
    vsize_(10),
    vused_(0)       // nothing used yet
{
}

template<class T> 
Stack<T>::~Stack()
{
  delete[] v_;      // this can't throw
}


	template<class T> 
T* NewCopy( const T* src,
            size_t   srcsize,
            size_t   destsize )
{
  assert( destsize >= srcsize );
  T* dest = new T[destsize];
  try
  {
    copy( src, src+srcsize, dest );
  }
  catch(...)
  {
    delete[] dest; // this can't throw
    throw;         // rethrow original exception
  }
  return dest;
}

template<class T> 
Stack<T>::Stack( const Stack<T>& other )
  : v_(NewCopy( other.v_,
                other.vsize_,
                other.vsize_ )),
    vsize_(other.vsize_),
    vused_(other.vused_)
{
}

template<class T> 
Stack<T>&
Stack<T>::operator=( const Stack<T>& other )
{
  if( this != &other )
  {
    T* v_new = NewCopy( other.v_,
                        other.vsize_,
                        other.vsize_ );
    delete[] v_;  // this can't throw
    v_ = v_new;   // take ownership
    vsize_ = other.vsize_;
    vused_ = other.vused_;
  }
  return *this;   // safe, no copy involved
}


	template<class T> 
size_t Stack<T>::Count() const
{
  return vused_;  // safe, builtins don't throw
}





template<class T> 
void Stack<T>::Push( const T& t )
{
  if( vused_ == vsize_ )  // grow if necessary
  {                       // by some grow factor
    size_t vsize_new = vsize_*2+1;
    T* v_new = NewCopy( v_, vsize_, vsize_new );
    delete[] v_;  // this can't throw
    v_ = v_new;   // take ownership
    vsize_ = vsize_new;
  }
  v_[vused_] = t;
  ++vused_;
}


		
	
	
	template<class T> 
void Stack<T>::Pop( T& result )
{
  if( vused_ == 0)
  {
    throw "pop from empty stack";
  }
  else
  {
    result = v_[vused_-1];
    --vused_;
  }
}


	
	template<class T> 
	const T& Stack<T>::Top() const
	{
	  if( vused_ == 0)
	  {
	    throw "empty stack";
	  }
	  else
	  {
	    return v_[vused_-1];
	  }
	}
	
	//to provide Top for const Stack objects, and:
	
	template<class T> 
	bool Stack<T>::Empty() const
	{
	  return( vused_ == 0 );
	}
	
	
	
  //try catch可能会引起代码的速度变慢 
  //NewCopy使用无try catch的写法
  template<class T> 
T* NewCopy( const T* src,
            size_t   srcsize,
            size_t   destsize )
{
  destsize=max(srcsize,destsize);//basic parm check
  strucy Janitor{
  	Janitor(T* p):pa(p){}
  	~Janitor()
  	{
  		if(uncaught_exception() delete[] pa;
  		T* pa;
  	}
  	T* dest=new T[destsize];
  	//if we go here ,the allocation/ctors were okay
  	Janitor j(dest);
  	copy(src,src+srcsize,destsize);
  	//if we got here ,the copy was okay...otherwise ,j
  	//was destroyed during stack unwinding and will handle 
  	//the cleanup of dest to avoid leaking memory
  };
}

//不发生异常的情况下 带try catch的语句的速度比上面的写法要快

