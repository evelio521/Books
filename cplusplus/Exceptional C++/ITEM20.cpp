ITEM20:class����
	 //�޸����д���
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
	
	 //���׼��
	 1.�����ظ����ó���ʽ�Ĵ���--�ر��Ǳ�׼���--����Ҫ�����Լ�׫д���룬���������죬���Ұ�ȫ
	 2.С����ʽת����������ʽ��ʱ����������ⶫ����һ���ð취���Ǿ����ܵ��ù��캯����Ϊexplicit
	 3.��������by const& (����by value)�ķ�ʽ����ֵ
	 4.������д a op= b ����Ҫд��  a=a op b �����������Ƚ������������Ч��
	 5.������ṩ��ĳ�������ӵı�׼��(��operator+)����ͬʱΪ���ṩһ��assign�汾�����Ժ���ʵ��ǰ��
	 6.ʹ��һ��׼��������һ��������Ӧ����member function��Ӧ����non-member functions
	    6.1 һԪ������Ӧ����members
	    6.2 = () [] -> ������members
	    6.3 assignment�汾�������� += -= /= *= Ӧ����members
	    6.4 �������е�Ԫ����Ӧ����nonmember
	 7.������operator<< �� operator>>����ʽ���� stream reference 
	 8.Ϊ��һ���ԣ���������ǰ��ʽ�ۼ�����Ϊ����ʵ��������ʽ���ۼ����ӡ�
		
		//�޸ĺ�Ĵ���
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
		
