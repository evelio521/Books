ITEM4-5:�������ɸ����Ե�ͨ��Container
	//Ϊ����Ķ�����vector classʵ�ֿ�����������Ϳ�����ֵ�������ṩ���Ŀ�����
	//�����޸Ĵ�����������֣�Ŀ�Ĳ��Ǽ���ģ��⡣
	  template<typename T, size_t size> 
		class fixed_vector
		{
		public:
		  typedef T*       iterator;
		  typedef const T* const_iterator;
		  iterator       begin()       { return v_; }
		  iterator       end()         { return v_+size; }
		  const_iterator begin() const { return v_; }
		  const_iterator end()   const { return v_+size; }
		
		private:
		  T v_[size];
		};
		//�������
		template<typename T, size_t size> 
		class fixed_vector
		{
		public:
		  typedef T*       iterator;
		  typedef const T* const_iterator;
		  fixed_vector() { }
		
		  template<typename O, size_t osize>
		  fixed_vector( const fixed_vector<O,osize>& other )
		  {
		    copy( other.begin(),
		          other.begin()+min(size,osize),
		          begin() );
		  }
		
		  template<typename O, size_t osize>
		  fixed_vector<T,size>&
		  operator=( const fixed_vector<O,osize>& other )
		  {
		    copy( other.begin(),
		          other.begin()+min(size,osize),
		          begin() );
		    return *this;
		  }
		
		  iterator       begin()       { return v_; }
		  iterator       end()         { return v_+size; }
		  const_iterator begin() const { return v_; }
		  const_iterator end()   const { return v_+size; }
		
		private:
		  T v_[size];
		};
		
		//
		struct X 
		{
		  template<typename T>
		  X( const T& );    // NOT copy constructor, T can't be X
		                    //��������ԭ��������������
		
		  template<typename T>
		  operator=( const T& );
		                    // NOT copy assignment, T can't be X
		                    //��������ԭ��������ֵ����
		};
		//��������ǵĽ���У���û������ԭ���Ŀ�������Ϳ�����ֵ
		//������д�Ĵ���ֻ�������˴���Ŀ���Ӧ��
		fixed_vector<char,4> v; 
		fixed_vector<int,4>  w;
		fixed_vector<int,4>  w2(w);
		        // calls implicit copy constructor
		fixed_vector<int,4>  w3(v);
		        // calls templated conversion constructor
		w = w2; // calls implicit copy assignment operator
		w = v;  // calls templated assignment operator


		//����д�Ĵ������;
		1.֧�ֿɱ�����(�����̳�����)
		  fixed_vector<char,4> v; 
			fixed_vector<int,4>  w(v);  // templated construction
			w = v;                      // templated assignment
			
			class B            { /*...*/ };
			class D : public B { /*...*/ };
			
			fixed_vector<D*,4> x;
			fixed_vector<B*,4> y(x);    // templated construction
			y = x;                      // templated assignment


		2.֧�ֿɱ�Ĵ�С
		  fixed_vector<char,6> v; 
			fixed_vector<int,4>  w(v);  // initializes using 4 values
			w = v;                      // assigns using 4 values
			class B            { /*...*/ };
			class D : public B { /*...*/ };
			fixed_vector<D*,16> x;
			fixed_vector<B*,42> y(x);   // initializes using 16 values
			y = x;                      // assigns using 16 values

			
		
		//����Ľ������  ������ʵ����
		1.Alternative: The Standard Library Approach
			//I happen to like the syntax and usability of the preceding functions, 
			//but there are still some nifty things they won��t let you do. Consider 
			//another approach that follows the style of the standard library.

		//Copying

			template<class RAIter> 
			fixed_vector( RAIter first, RAIter last )
			{
			  copy( first,
			        first+min(size,(size_t)last-first),
			        begin() );
			}
			
			//Now when copying, instead of writing:
			
			fixed_vector<char,6> v; 
			fixed_vector<int,4>  w(v);  // initialize using 4 values
			
			//we need to write:
			
			fixed_vector<char,6> v; 
			fixed_vector<int,4>  w(v.begin(), v.end());
			                            // initialize using 4 values
			
			 2.Assignment

				//Note that we can��t templatize assignment to take an iterator range, 
				//because operator=() may take only one parameter. Instead, we can 
				//provide a named function:

				template<class Iter> 
				fixed_vector<T,size>&
				assign( Iter first, Iter last )
				{
				  copy( first,
				        first+min(size,(size_t)last-first),
				        begin() );
				  return *this;
				}
				
				//Now when assigning, instead of writing:
				
				w = v;                      // assign using 4 values 
				
				//we need to write:
				
				w.assign(v.begin(), v.end()); 
				                            // assign using 4 value
				
				//assign���Ǳ���ģ�����Ҳ������
				copy( v.begin(), v.begin()+4, w.begin() ); //��assignҪ��

				//һ���㶨���κ���ʽ�Ĺ��캯�����������Ͳ�����Ϊ�㶨�幹�캯��
				//�����ڵ�һ�ֽⷨʱ��������ȻҪ��ʾ�Ķ��忽�����캯��			
					
					
				//���ֽⷨ�У���һ��������ʹ�ã��ڶ����Ŀ����Ը���		
