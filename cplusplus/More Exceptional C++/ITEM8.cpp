ITEM8:使用set和map
	
	//What‘s wrong with the following code? How would you correct it?

		map<int,string>::iterator i = m.find( 13 );
		if( i != m.end() )
		{
		  const_cast<int&>( i->first ) = 9999999;
		}
		
		//To what extent are the problems fixed by writing the following instead?
		
		map<int,string>::iterator i = m.find( 13 );
		if( i != m.end() )
		{
		  string s = i->second;
		  m.erase( i );
		  m.insert( make_pair( 9999999, s ) );
		}
		
		//可以通过set::iterator修改set的内容吗？ 
		
		//关联式容器的一个基本准则
		  //一旦一个键值被插入容器。那么无论键值被什么方式修改，他在容器的相对位置不会改变。
		  
		//关联式容器的编程规范
		1.规定const就是const
		  //对于map和multimap，键是不允许修改的，map<key,value>::iterator指向的是
		  //pair<const key,value>,也就是说你只能改变值部分，
		2.规定总是以先删除再重复插入的方式对键值进行修改
		
		  //To what extent are the problems fixed by writing the following instead?
		
		map<int,string>::iterator i = m.find( 13 );
		if( i != m.end() )
		{
		  string s = i->second;
		  m.erase( i );
		  m.insert( make_pair( 9999999, s ) );
		}
		//遗憾的是，这个解决方案并不是很好
		1.假设key类型具有某种外部可见状态，这个状态值可以被其他代码得到
		  --例如一个指针指向缓冲区，系统的其他部分无需操作key就可以修改
		  这一缓冲区。在假如，那个外部可见的状态参与了compare的比较操作
		  那么，即使对key对象一无所知，即使对使用这个关联式容器的代码一
		  无所知，如果对外部可见的状态值进行修改，还是可以改变键值的相对
		  顺序
		2.假设key的类型为String:compare类型将这个key解释为文件名，在做比
			较	时，比较的对象是文件的内容。这种情况下，即使键友元没有被修改，
			但是文件被另一个进程修改，建的相对次序还是可以被修改
			
			
	//可以通过set::iterator修改set的内容吗？ 
	
	//set::iterator和set::const_iterator是常量迭代器，想要修改set
	//的对象，必须使用const_cast
	
	