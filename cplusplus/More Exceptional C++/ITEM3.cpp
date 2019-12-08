ITEM3:状态带来的问题
	//1.什么是predicate
	
	// Example 3-1(a): Using a unary predicate // 一元predicate
		//
		if( pred( *first ) )
		{
		  /* ... */
		}
		// Example 3-1(b): Using a binary predicate// 二元predicate
		//
		if( bpred( *first1, *first2 ) )
		{
		  /* ... */
		}
		//例子
		// Example 3-1(c): A sample find_if()
			//
			template<typename Iter, typename Pred> inline
			Iter find_if( Iter first, Iter last, Pred pred )
			{
			  while( first != last && !pred(*first) )
			  {
			    ++first;
			  }
			  return first;
			}
			//通过函数指针来使用find_if
			// Example 3-1(d):
			// Using find_if() with a function pointer.
			//
			bool GreaterThanFive( int i )
			{
			  return i > 5;
			}
			
			bool IsAnyElementGreaterThanFive( vector<int>& v )
			{
			  return find_if( v.begin(), v.end(), GreaterThanFive )
			         != v.end();
			}
			//通过函数对象使用find_if
			// Example 3-1(e):
			// Using find_if() with a function object.
			//
			class GreaterThanFive
			  : public std::unary_function<int, bool>
			{
			public:
			  bool operator()( int i ) const
			  {
			    return i > 5;
			  }
			};
			bool IsAnyElementGreaterThanFive( vector<int>& v )
			{
			  return find_if( v.begin(), v.end(), GreaterThanFive() )
			         != v.end();
			}
			
			//2.状态性predicate何时可用？
			// Example 3-2(a):
			// Using find_if() with a more general function object.
			//
			class GreaterThan
			  : public std::unary_function<int, bool>
			{
			public:
			  GreaterThan( int value ) : value_( value ) { }
			  bool operator()( int i ) const
			  {
			    return i > value_;
			  }
			private:
			  const int value_;
			};
			
			bool IsAnyElementGreaterThanFive( vector<int>& v )
			{
			  return find_if( v.begin(), v.end(), GreaterThan(5) )
			         != v.end();
			}
			//比上面的例子更具有通用性
			// Example 3-2(b):
			// Using find_if() with a fully general function object.
			//
			template<typename T>
			class GreaterThan
			  : public std::unary_function<T, bool>
			{
			public:
			  GreaterThan( T value ) : value_( value ) { }
			
			  bool operator()( const T& t ) const
			  {
			    return t > value_;
			  }
			
			private:
			  const T value_;
			};
			
			bool IsAnyElementGreaterThanFive( vector<int>& v )
			{
			  return find_if( v.begin(), v.end(), GreaterThan<int>(5) )
			         != v.end();
			}
			
			//写一个函数对象，当他使用n次后返回true
			// Example 3-2(c)
			// (From Item 2, Example 2-3(b))
			//
			// Method 2: Write a function object which returns
			// true the nth time it's applied, and use
			// that as a predicate for remove_if.
			//
			class FlagNth
			{
			public:
			  FlagNth( size_t n ) : current_(0), n_(n) { }
			
			  template<typename T>
			  bool operator()( const T& ) { return ++current_ == n_; }
			
			private:
			  size_t       current_;
			  const size_t n_;
			};
			//状态性predicate和非状态性predicate的区别:
			//对于状态性predicate，拷贝和他本身不是等同的,
			//而非状态性predicate，拷贝和他本身是相同的。
			
			//3.为了让状态性的predicate正常工作，随算法的要求
			
			1.算法绝不能对predicate做赋值
			2.算法必须“以某个已知顺序”，将predicate作用到区间上的元素
			//状态性predicate一般不能和标准协同工作，建议少用
			//一个不完整方案，拷贝之间共享状态
			// Example 3-3(a): A (partial) solution
			// that shares state between copies.
			//
			class FlagNthImpl
			{
			public:
			  FlagNthImpl( size_t nn ) : i(0), n(nn) { }
			  size_t       i;
			  const size_t n;
			};
			
			class FlagNth
			{
			public:
			  FlagNth( size_t n )
			    : pimpl_( new FlagNthImpl( n ) )
			  {
			  }
			
			  template<typename T>
			  bool operator()( const T& )
			  {
			    return ++(pimpl_->i) == pimpl_->n;
			  }
			
			private:
			  CountedPtr<FlagNthImpl> pimpl_;
			};
			//CountPtr的实现
			template<typename T>
			class CountedPtr
			{
			private:
			  class Impl
			  {
			  public:
			    Impl( T* pp ) : p( pp ), refs( 1 ) { }
			
			    ~Impl() { delete p; }
			
			    T*     p;
			    size_t refs;
			  };
			  Impl* impl_;
			public:
			  explicit CountedPtr( T* p )
			    : impl_( new Impl( p ) ) { }
			
			  ~CountedPtr() { Decrement(); }
			
			  CountedPtr( const CountedPtr& other )
			    : impl_( other.impl_ )
			  {
			    Increment();
			  }
			
			  CountedPtr& operator=( const CountedPtr& other )
			  {
			    if( impl_ != other.impl_ )
			    {
			      Decrement();
			      impl_ = other.impl_;
			      Increment();
			    }
			    return *this;
			  }
			
			  T* operator->() const
			  {
			    return impl_->p;
			  }
			
			  T& operator*() const
			  {
			    return *(impl_->p);
			  }
			
			private:
			  void Decrement()
			  {
			    if( --(impl_->refs) == 0 )
			    {
			      delete impl_;
			    }
			  }
			
			  void Increment()
			  {
			    ++(impl_->refs);
			  }
			};
			
			
						
												
