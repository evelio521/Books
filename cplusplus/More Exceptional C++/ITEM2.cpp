ITEM2:Prediacte,之一:remove()删除了什么
	//1.removed()算法完成什么功能
	//remove()并没有将对象从容器中删除，remove()执行完成后，容器的大小不变。
	//简单的说remove()只是用未删除的对象填补已删除的对象留下的缺口，每一个被
	//删除的对象在尾部还说有相应的死亡对象。remove()返回指向第一个死亡的对象的
	//迭代器。如果没有对象被删除，remove()返回end()迭代器
	//For example, consider a vector<int> v that contains the following nine elements:

			1 2 3 1 2 3 1 2 3

		Say that you used the following code to try to remove all 3‘s from the container:
		
		// Example 2-1
		//
		remove( v.begin(), v.end(), 3 );  // subtly wrong
		//What would happen? The answer is something like this:
		1 2 1 2 1 2 ？ ？ ？
		
		//2. 写一段代码用来删除std::vector<int>中值为3的所有元素
		// Example 2-2: Removing 3's from a vector<int> v

		v.erase( remove( v.begin(), v.end(), 3 ), v.end() );
		
		
		//3.为删除容器中的第n个元素，写一段代码
		// Example 2-3(a): Solving the problems
		//
		// Precondition:
		//  - n must not exceed the size of the range
		//
		template<typename FwdIter>
		FwdIter remove_nth( FwdIter first, FwdIter last, size_t n )
		{
		  // Check precondition. Incurs overhead in debug mode only.
		  assert( distance( first, last ) >= n );
		
		  // The real work.
		  advance( first, n );
		  if( first != last )
		  {
		    FwdIter dest = first;
		    return copy( ++first, last, dest );
		  }
		  return last;
		}
		// Example 2-3(b)
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
		// Example invocation
		... remove_if( v.begin(), v.end(), FlagNth(3) )
		
		//Example 2-3(a)的优点：
		1.他是正确的
		2.利用了iterator traits特性，特别是迭代器类别
		  因而在随机访问的迭代器身上表现更加
		//Example 2-3(b)的缺点：
		 将在本短系列的第二部分详细分析
		  
		
				
				
						
				
				
				
						
