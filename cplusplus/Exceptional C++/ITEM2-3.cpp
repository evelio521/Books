ITEM2:�����ִ�Сд��string
	//дһ�������ִ�Сд��string�ͱ�
	
	ci_string s( "AbCdE" ); 
	// case insensitive
	//
	assert( s == "abcde" );
	assert( s == "ABCDE" );
	// still case-preserving, of course
	//
	assert( strcmp( s.c_str(), "AbCdE" ) == 0 );
	assert( strcmp( s.c_str(), "abcde" ) != 0 );

	//����ʵ��
	 
	 //���ִ�Сд��string
	 template<class charT, 
         class traits = char_traits<charT>,
         class Allocator = allocator<charT> >
	 class basic_string;
		//����char_traits�ṩ������µ��ַ����ȽϺ���
		1.eq() 		  ���
		2.ne()      ����
		3.lt()      С��
		4.compare() �Ƚ��ַ�����
		5.find()    �����ַ�����
	//���ϴ���
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
	
	//���ֵ�����
	1.//����Ĵ����޷�ͨ������
	  ci_string s="abc";
	  cout<<s<<endl;
	  //����취
	   cout<<s.c_str()<<endl;
	2.//����Ĵ����޷�ͨ������ ci_string ��stringʹ�� + += =
	  string a="aaa";
	  ci_string b="bbb";
	  string c=a+b;
	  //����취�Ƕ�����Щ��������
	  string c=a+b.c_str();
	3.������char_traits<char>�̳г�ci_char_traits�ǰ�ȫ��
