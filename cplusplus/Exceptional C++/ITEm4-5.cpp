ITEM4-5:具有最大可复用性的通过Container
	//为下面的定长的vector class实现拷贝构造操作和拷贝赋值操作，提供最大的可用性
	//不能修改代码的其他部分，目的不是加入模版库。
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
		//解决方案
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
		                    //不会隐藏原来拷贝构造声明
		
		  template<typename T>
		  operator=( const T& );
		                    // NOT copy assignment, T can't be X
		                    //不会隐藏原来拷贝赋值声明
		};
		//因此在我们的解答中，并没有隐藏原来的拷贝构造和拷贝赋值
		//我们手写的代码只是增加了代码的可适应性
		fixed_vector<char,4> v; 
		fixed_vector<int,4>  w;
		fixed_vector<int,4>  w2(w);
		        // calls implicit copy constructor
		fixed_vector<int,4>  w3(v);
		        // calls templated conversion constructor
		w = w2; // calls implicit copy assignment operator
		w = v;  // calls templated assignment operator


		//我们写的代码的用途
		1.支持可变类型(包括继承在内)
		  fixed_vector<char,4> v; 
			fixed_vector<int,4>  w(v);  // templated construction
			w = v;                      // templated assignment
			
			class B            { /*...*/ };
			class D : public B { /*...*/ };
			
			fixed_vector<D*,4> x;
			fixed_vector<B*,4> y(x);    // templated construction
			y = x;                      // templated assignment


		2.支持可变的大小
		  fixed_vector<char,6> v; 
			fixed_vector<int,4>  w(v);  // initializes using 4 values
			w = v;                      // assigns using 4 values
			class B            { /*...*/ };
			class D : public B { /*...*/ };
			fixed_vector<D*,16> x;
			fixed_vector<B*,42> y(x);   // initializes using 16 values
			y = x;                      // assigns using 16 values

			
		
		//另外的解决方案  更具有实用性
		1.Alternative: The Standard Library Approach
			//I happen to like the syntax and usability of the preceding functions, 
			//but there are still some nifty things they won‘t let you do. Consider 
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

				//Note that we can’t templatize assignment to take an iterator range, 
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
				
				//assign不是必须的，我们也可以用
				copy( v.begin(), v.begin()+4, w.begin() ); //比assign要好

				//一旦你定义任何形式的构造函数，编译器就不会在为你定义构造函数
				//所以在第一种解法时，我们仍然要显示的定义拷贝构造函数			
					
					
				//两种解法中，第一个的容易使用，第二个的可用性更好		
