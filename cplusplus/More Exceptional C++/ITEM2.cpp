ITEM2:Prediacte,֮һ:remove()ɾ����ʲô
	//1.removed()�㷨���ʲô����
	//remove()��û�н������������ɾ����remove()ִ����ɺ������Ĵ�С���䡣
	//�򵥵�˵remove()ֻ����δɾ���Ķ������ɾ���Ķ������µ�ȱ�ڣ�ÿһ����
	//ɾ���Ķ�����β����˵����Ӧ����������remove()����ָ���һ�������Ķ����
	//�����������û�ж���ɾ����remove()����end()������
	//For example, consider a vector<int> v that contains the following nine elements:

			1 2 3 1 2 3 1 2 3

		Say that you used the following code to try to remove all 3��s from the container:
		
		// Example 2-1
		//
		remove( v.begin(), v.end(), 3 );  // subtly wrong
		//What would happen? The answer is something like this:
		1 2 1 2 1 2 �� �� ��
		
		//2. дһ�δ�������ɾ��std::vector<int>��ֵΪ3������Ԫ��
		// Example 2-2: Removing 3's from a vector<int> v

		v.erase( remove( v.begin(), v.end(), 3 ), v.end() );
		
		
		//3.Ϊɾ�������еĵ�n��Ԫ�أ�дһ�δ���
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
		
		//Example 2-3(a)���ŵ㣺
		1.������ȷ��
		2.������iterator traits���ԣ��ر��ǵ��������
		  �����������ʵĵ��������ϱ��ָ���
		//Example 2-3(b)��ȱ�㣺
		 ���ڱ���ϵ�еĵڶ�������ϸ����
		  
		
				
				
						
				
				
				
						
