ITEM7:使用标准库
	//对于修改后的版本若然有一些问题
	
	string FindAddr( const list<Employee>& emps, 
                 const string&         name )
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

  //应该用标准库
  //Solution
 
		//With no other changes, simply using the standard find() 
		//algorithm could have avoided two temporaries, as well as
		// the emps.end() recomputation inefficiency from the original 
		//code. For the best effect to reduce temporaries, provide an
		// operator==() taking an Employee& and a name string&.

		string FindAddr( list<Employee> emps, string name ) 
		{
		  list<Employee>::iterator i(
		    find( emps.begin(), emps.end(), name )
		    );
		  if( i != emps.end() )
		  {
		    return i->addr;
		  }
		  return "";
		}
		//在进行一些修改
		
		string FindAddr( const list<Employee>& emps, 
                 const string& name )
		{
		  list<Employee>::const_iterator i(
		    find( emps.begin(), emps.end(), name )
		    );
		  if( i != emps.end() )
		  {
		    return i->addr;
		  }
		  return "";
		}


 
