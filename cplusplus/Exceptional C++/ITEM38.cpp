ITEM38:对象等同问题
	//防止自我赋值，下面的代码是充分必要的吗？
	T& T::operator=( const T& other ) 
	{
	  if( this != &other )    // the test in question
	  {
	    // ...
	  }
	  return *this;
	}
	//从技术上看，它既不是必要的也不是充分的。在实践当中，他工作的颇好但是
	//也有可能在C++标准中被修改
