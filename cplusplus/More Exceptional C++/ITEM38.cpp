ITEM38:typedef
	//1.Ϊʲôʹ��typedef,�����ܵ��о�ʹ�ó��Ϻ����ɡ�
	  /*
	   * 1.���ڴ��֣�����Խ�̣�����Խ���ס�
	   * 2.typedef���ڴ����Ķ�
	   *   int ( *t(int) )( int* );
     *   ��type�ļ򻯺�Ϊ:
     *   typedef int (*Func)( int* );
     *   Func t( int );
     *   ����һ�������������˺���������Ϊt���������Ϊint,����ֵΪһ������
     *   ָ�룬ָ��һ���������Ϊint������ֵΪint�ĺ���
	   * 3.���ڽ�����typedef�����ڱ��������ͼ
	   *   int x;
		 *	 int y;
		 *			y = x * 3;   // might be okay -- who knows?
		 *		
		 *		�Ƚ�����Ĵ���
		 *		
		 *		typedef int Inches;
		 *		typedef int Dollars;
		 *    Inches  x;
		 *		Dollars y;
		 *		y = x * 3;   // hmmm...?
		 * 4.����ֲ�ԣ�������Щ��ƽ̨��ص����ƻ򲻾��п���ֲ�Ե�����ʹ��typedef
		 *    #if defined USING_COMPILER_A
		 *       typedef __int32 Int32;
		 *       typedef __int64 Int64;
		 *    #elif defined USING_COMPILER_B
		 *       typedef int       Int32;
		 *       typedef long long Int64;
     *    #endif
     * /


	//2.���õ��˱�׼�����Ĵ����У�ʹ��typedefΪʲô�Ǹ������⣿
	5.����ԣ���֮���������޸����ƣ�����һ���޸�typedef������
    /*
     *   void f( vector<Customer>& custs )
		 *   {
		 *      vector<Customer>::iterator i = custs.begin();
		 *       ...
		 *    }
     *   �����������Ƚ�
     *    typedef vector<Customer> Customers;
		 *    typedef Customers::iterator CustIter;
		 *
		 *     ...
		 *
		 *     void f( Customers& custs )
		 *     {
		 *       CustIter i = custs.begin();
		 *       ...
		 *      }
		 */
		