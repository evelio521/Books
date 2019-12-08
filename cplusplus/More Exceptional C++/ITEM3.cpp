ITEM3:״̬����������
	//1.ʲô��predicate
	
	// Example 3-1(a): Using a unary predicate // һԪpredicate
		//
		if( pred( *first ) )
		{
		  /* ... */
		}
		// Example 3-1(b): Using a binary predicate// ��Ԫpredicate
		//
		if( bpred( *first1, *first2 ) )
		{
		  /* ... */
		}
		//����
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
			//ͨ������ָ����ʹ��find_if
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
			//ͨ����������ʹ��find_if
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
			
			//2.״̬��predicate��ʱ���ã�
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
			//����������Ӹ�����ͨ����
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
			
			//дһ���������󣬵���ʹ��n�κ󷵻�true
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
			//״̬��predicate�ͷ�״̬��predicate������:
			//����״̬��predicate���������������ǵ�ͬ��,
			//����״̬��predicate������������������ͬ�ġ�
			
			//3.Ϊ����״̬�Ե�predicate�������������㷨��Ҫ��
			
			1.�㷨�����ܶ�predicate����ֵ
			2.�㷨���롰��ĳ����֪˳�򡱣���predicate���õ������ϵ�Ԫ��
			//״̬��predicateһ�㲻�ܺͱ�׼Эͬ��������������
			//һ������������������֮�乲��״̬
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
			//CountPtr��ʵ��
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
			
			
						
												
