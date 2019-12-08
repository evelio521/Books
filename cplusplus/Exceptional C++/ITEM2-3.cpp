ITEM2:不区分大小写的string
	//写一个不区分大小写的string型别
	
	ci_string s( "AbCdE" ); 
	// case insensitive
	//
	assert( s == "abcde" );
	assert( s == "ABCDE" );
	// still case-preserving, of course
	//
	assert( strcmp( s.c_str(), "AbCdE" ) == 0 );
	assert( strcmp( s.c_str(), "abcde" ) != 0 );

	//具体实现
	 
	 //区分大小写的string
	 template<class charT, 
         class traits = char_traits<charT>,
         class Allocator = allocator<charT> >
	 class basic_string;
		//其中char_traits提供了如果下的字符串比较函数
		1.eq() 		  相等
		2.ne()      不等
		3.lt()      小于
		4.compare() 比较字符序列
		5.find()    搜索字符序列
	//整合代码
	struct ci_char_traits : public char_traits<char> 
              // just inherit all the other functions
              //  that we don't need to replace
  {
    static bool eq( char c1, char c2 )
     { return toupper(c1) == toupper(c2); }
    static bool lt( char c1, char c2 )
     { return toupper(c1) <  toupper(c2); }
    static int compare( const char* s1,
                        const char* s2,
                      size_t n )
    { return memicmp( s1, s2, n ); }
           // if available on your platform,
           //  otherwise you can roll your own
    static const char* find( const char* s, int n, char a )
      {
    while( n-- > 0 && toupper(*s) != toupper(a) )
    {
        ++s;
     }
      return n >= 0 ? s : 0;
   }
  };

  //And finally, the key that brings it all together:

   typedef basic_string<char, ci_char_traits> ci_string;
	
	//出现的问题
	1.//下面的代码无法通过编译
	  ci_string s="abc";
	  cout<<s<<endl;
	  //解决办法
	   cout<<s.c_str()<<endl;
	2.//下面的代码无法通过编译 ci_string 和string使用 + += =
	  string a="aaa";
	  ci_string b="bbb";
	  string c=a+b;
	  //解决办法是定义这些操作符或
	  string c=a+b.c_str();
	3.这样从char_traits<char>继承出ci_char_traits是安全的
