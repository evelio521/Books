ITEM6:临时对象
	//找出下面临时对象使用的地方
	string FindAddr( list<Employee> emps, string name ) //两个参数都会产生临时对象
		{
		  for( list<Employee>::iterator i = emps.begin();
		       i != emps.end();
		       i++ )//临时对象产生
		  {
		    if( *i == name )//临时对象产生
		    {
		      return i->addr;
		    }
		  }
		  return "";//临时对象产生
		}
	
	  //注意以下几点
	  1.请使用const&，而不是传值拷贝
	  2.请使用++i 避免使用i++
	  3.时刻注意因为参数转换操作而产生的隐藏的临时对象，
	    一个避免他好的方法是使用显示的构造函数
	  4.绝对绝对不要返回绝不对象的引用

  //对于上面代码的优化
  string FindAddr( const list<Employee>& emps, 
                   const string& name )
 {
  list<Employee>::const_iterator end( emps.end() );
  for( list<Employee>::const_iterator i = emps.begin();
       i != end;
       ++i )
  {
    if( i->name == name )
    {
      return i->addr;
    }
  }
  return "";
 }

