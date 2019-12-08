ITEM43:��ȷʹ��const
	//���������constʹ���Ƿ���ȷ
	class Polygon 
	{
	public:
	  Polygon() : area_(-1) {}
	  void  AddPoint( const Point pt ) 
	  { //1.��ΪPoint������ô�ֵ��ʽ����������Ϊconst����û����ˮ
	  	InvalidateArea();
	    points_.push_back(pt); 
	  }
	  Point GetPoint( const int i )   
	 { //2.��ΪPoint������ô�ֵ��ʽ����������Ϊconst����û����ˮ�������������
	 	 //3.�����Ա����Ӧ����const����Ϊ���ı�����״̬
	 	 //4.����ú����ķ������Ͳ���һ���ڽ����͵Ļ���ͨ��Ӧ�ý��䷵������Ҳ����Ϊconst
	 	 //�����������ڸú����ĵ����ߣ���Ϊ���ǵı������ܹ��ں�����������ͼ�޸���ʱ����
	 	 //ʱ����һ�����󣬴Ӷ��ﵽ����Ŀ�ġ����������޸���ʱ������Ӧ����GetPoint
	 	 //����һ�����á�
	 		return points_[i]; 
	 }
	  int   GetNumPoints()            
	  { //5.��������Ӧ������Ϊconst
	  		return points_.size();
	  }
	  double GetArea()
	  {//6.������������޸��˶�����ڲ�״̬�����ǻ���Ӧ������Ϊconst,��Ϊ���޸Ķ���Ŀɼ�
	  	//״̬û�����仯������ζ��area_Ӧ�ñ�����Ϊmutable.��סһ���ú�������Ϊconst
	    if( area_ < 0 ) // if not yet calculated and cached
	    {
	      CalcArea();     // calculate now
	    }
	    return area_;
	  }
	private:
	  void InvalidateArea() 
	  { //7.�������Ӧ������Ϊconst
	  	area_ = -1; 
	  }
	  void CalcArea()
	  {//8.�����Ա��������Ӧ�ñ�����Ϊconst,������ô˵�������ٻᱻ����һ��const��Ա����GetArea()����
	    area_ = 0;
	    vector<Point>::iterator i;
	    	//9.��Ȼiterator����ı�points_����״̬����������Ӧ����һ��const_iterator
	    for( i = points_.begin(); i != points_.end(); ++i )
	      area_ += /* some work */;
	  }
	  vector<Point> points_;
	  double        area_;
	};
	Polygon operator+( Polygon& lhs, Polygon& rhs )
	{//10.����Ӧ����const���ô���
		//11.����ֵӦ����const
	  Polygon ret = lhs;
	  int last = rhs.GetNumPoints();
	  //12.��ȻlastҲ�Ӳ���ı䣬��ôӦ��Ϊconst int����
	  for( int i = 0; i < last; ++i ) // concatenate
	  {
	    ret.AddPoint( rhs.GetPoint(i) );
	  }
	  return ret;
	}
	
	void f( const Polygon& poly )
	{//13.�����const�������ã���Ϊ�������һ�������ǲ����ܱ��ı䣬ʹ��ָ����һ������
	  const_cast<Polygon&>(poly).AddPoint( Point(0,0) );
	}
	
	void g( Polygon& const rPoly ) 
	{ //13.�����const�������ã���Ϊ�������һ�������ǲ����ܱ��ı䣬ʹ��ָ����һ������
		rPoly.AddPoint( Point(1,1) ); 
		}
	
	void h( Polygon* const pPoly ) 
	{ //14.�����constû���ã���ָ��ʹ�ô�ֵ��ʽ��������
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
	
	//ѧϰָ�����ں����ڶԷ��ڽ������Ͳ���ֵ���ط���ʱ������ú�������һ��constֵ
  //�޸ĺ�Ĵ��� ��Ҫ���Ǵ�����ĺû�
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
	
