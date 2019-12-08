ITEM43:正确使用const
	//下面持续的const使用是否正确
	class Polygon 
	{
	public:
	  Polygon() : area_(-1) {}
	  void  AddPoint( const Point pt ) 
	  { //1.因为Point对象采用传值方式，所以声明为const几乎没有油水
	  	InvalidateArea();
	    points_.push_back(pt); 
	  }
	  Point GetPoint( const int i )   
	 { //2.因为Point对象采用传值方式，所以声明为const几乎没有油水，反而容器误解
	 	 //3.这个成员函数应该是const，因为不改变对象的状态
	 	 //4.如果该函数的返回类型不是一个内建类型的话，通常应该将其返回类型也声明为const
	 	 //这样做有利于该函数的调用者，因为它是的编译器能够在函数调用者企图修改临时变量
	 	 //时产生一个错误，从而达到保护目的。如果真的想修改临时变量，应该让GetPoint
	 	 //返回一个引用。
	 		return points_[i]; 
	 }
	  int   GetNumPoints()            
	  { //5.函数本身应该声明为const
	  		return points_.size();
	  }
	  double GetArea()
	  {//6.尽管这个函数修改了对象的内部状态，但是还是应该声明为const,因为被修改对象的可见
	  	//状态没发生变化。这意味着area_应该被声明为mutable.记住一定让函数声明为const
	    if( area_ < 0 ) // if not yet calculated and cached
	    {
	      CalcArea();     // calculate now
	    }
	    return area_;
	  }
	private:
	  void InvalidateArea() 
	  { //7.这个函数应该声明为const
	  	area_ = -1; 
	  }
	  void CalcArea()
	  {//8.这个成员函数绝对应该被声明为const,不管怎么说，它至少会被另外一个const成员函数GetArea()调用
	    area_ = 0;
	    vector<Point>::iterator i;
	    	//9.既然iterator不会改变points_集的状态，所以这里应该是一个const_iterator
	    for( i = points_.begin(); i != points_.end(); ++i )
	      area_ += /* some work */;
	  }
	  vector<Point> points_;
	  double        area_;
	};
	Polygon operator+( Polygon& lhs, Polygon& rhs )
	{//10.参数应该是const引用传递
		//11.返回值应该是const
	  Polygon ret = lhs;
	  int last = rhs.GetNumPoints();
	  //12.既然last也从不会改变，那么应该为const int类型
	  for( int i = 0; i < last; ++i ) // concatenate
	  {
	    ret.AddPoint( rhs.GetPoint(i) );
	  }
	  return ret;
	}
	
	void f( const Polygon& poly )
	{//13.这里的const毫无作用，因为无论如何一个引用是不可能被改变，使其指向另一个对象
	  const_cast<Polygon&>(poly).AddPoint( Point(0,0) );
	}
	
	void g( Polygon& const rPoly ) 
	{ //13.这里的const毫无作用，因为无论如何一个引用是不可能被改变，使其指向另一个对象
		rPoly.AddPoint( Point(1,1) ); 
		}
	
	void h( Polygon* const pPoly ) 
	{ //14.这里的const没有用，对指针使用传值方式毫无意义
		pPoly->AddPoint( Point(2,2) ); 
	}
	
	int main()
	{
	  Polygon poly;
	  const Polygon cpoly;
	  f(poly);
	  f(cpoly);
	  g(poly);
	  h(&poly);
	}
	
	//学习指导：在函数内对非内建的类型采用值返回方法时，最好让函数返回一个const值
  //修改后的代码 不要考虑代码风格的好坏
  class Polygon 
	{
	public:
	  Polygon() : area_(-1) {}
	
	  void        AddPoint( Point pt ) { InvalidateArea();
	                                     points_.push_back(pt); }
	
	  const Point GetPoint( int i ) const  { return points_[i]; }
	
	  int         GetNumPoints() const { return points_.size(); }
	
	  double GetArea() const
	  {
	    if( area_ < 0 ) // if not yet calculated and cached
	      CalcArea();     // calculate now
	    return area_;
	  }
	
	  private:
	    void InvalidateArea() const { area_ = -1; }
	
	    void CalcArea() const
	    {
	      area_ = 0;
	      vector<Point>::const_iterator i;
	      for( i = points_.begin(); i != points_.end(); ++i )
	        area_ += /* some work */;
	    }
	
	    vector<Point>  points_;
	    mutable double area_;
	  };
	
	  const Polygon operator+( const Polygon& lhs,
	                           const Polygon& rhs )
	  {
	    Polygon ret = lhs;
	    const int last = rhs.GetNumPoints();
	    for( int i = 0; i < last; ++i ) // concatenate
	      ret.AddPoint( rhs.GetPoint(i) );
	    return ret;
	  }
	  
	  void f( Polygon& poly )
	  {
	    poly.AddPoint( Point(0,0) );
	  }
	
	  void g( Polygon& rPoly ) { rPoly.AddPoint( Point(1,1) ); }
	  void h( Polygon* pPoly ) { pPoly->AddPoint( Point(2,2) ); }
	
	  int main()
	  {
	    Polygon poly;
	    f(poly);
	    g(poly);
	    h(&poly);
	  }
	
