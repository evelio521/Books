ITEM20:class技术
	 //修改下列代码
	 class Complex 
	{
	public:
	  Complex( double real, double imaginary = 0 )
	    : _real(real), _imaginary(imaginary)
	  {
	  }
	  void operator+ ( Complex other )
	  {
	    _real = _real + other._real;
	    _imaginary = _imaginary + other._imaginary;
	  }
	  void operator<<( ostream os )
	  {
	    os << "(" << _real << "," << _imaginary << ")";
	  }
	  Complex operator++()
	  {
	    ++_real;
	    return *this;
	  }
	  Complex operator++( int )
	  {
	    Complex temp = *this;
	    ++_real;
	    return temp;
	  }
	private:
	  double _real, _imaginary;
	};
	
	 //设计准则
	 1.尽量重复运用程序式的代码--特别是标准库的--而不要老想自己撰写代码，那样不但快，而且安全
	 2.小心隐式转换带来的隐式暂时物件。避免这东西的一个好办法就是尽可能的让构造函数成为explicit
	 3.尽量的以by const& (而非by value)的方式来传值
	 4.尽量的写 a op= b 而不要写成  a=a op b 。这样不但比较清楚，而且有效率
	 5.如果你提供了某个运算子的标准版(如operator+)，请同时为他提供一份assign版本。并以后者实现前者
	 6.使用一下准则来决定一个运算子应该是member function或应该是non-member functions
	    6.1 一元运算子应该是members
	    6.2 = () [] -> 必须是members
	    6.3 assignment版本的运算子 += -= /= *= 应该是members
	    6.4 其他所有的元算子应该是nonmember
	 7.总是在operator<< 和 operator>>函数式传回 stream reference 
	 8.为了一致性，请总是以前置式累加算子为本，实作出后置式的累加算子。
		
		//修改后的代码
		class Complex 
		{
		public:
		  explicit Complex( double real, double imaginary = 0 )
		    : real_(real), imaginary_(imaginary)
		  {
		  }
		
		  Complex& operator+=( const Complex& other )
		  {
		    real_ += other.real_;
		    imaginary_ += other.imaginary_;
		    return *this;
		  }
		
		  Complex& operator++()
		  {
		    ++real_;
		    return *this;
		  }
		
		  const Complex operator++( int )
		  {
		    Complex temp( *this );
		    ++*this;
		    return temp;
		  }
		
		  ostream& Print( ostream& os ) const
		  {
		    return os << "(" << real_ << "," << imaginary_ << ")";
		  }
		
		  private:
		    double real_, imaginary_;
		  };
		
		  const Complex operator+( const Complex& lhs, const Complex& rhs )
		  {
		    Complex ret( lhs );
		    ret += rhs;
		    return ret;
		  }
		
		  ostream& operator<<( ostream& os, const Complex& c )
		  {
		    return c.Print(os);
		  }
		
