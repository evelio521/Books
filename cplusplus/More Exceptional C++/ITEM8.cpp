ITEM8:ʹ��set��map
	
	//What��s wrong with the following code? How would you correct it?

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
		
		//����ͨ��set::iterator�޸�set�������� 
		
		//����ʽ������һ������׼��
		  //һ��һ����ֵ��������������ô���ۼ�ֵ��ʲô��ʽ�޸ģ��������������λ�ò���ı䡣
		  
		//����ʽ�����ı�̹淶
		1.�涨const����const
		  //����map��multimap�����ǲ������޸ĵģ�map<key,value>::iteratorָ�����
		  //pair<const key,value>,Ҳ����˵��ֻ�ܸı�ֵ���֣�
		2.�涨��������ɾ�����ظ�����ķ�ʽ�Լ�ֵ�����޸�
		
		  //To what extent are the problems fixed by writing the following instead?
		
		map<int,string>::iterator i = m.find( 13 );
		if( i != m.end() )
		{
		  string s = i->second;
		  m.erase( i );
		  m.insert( make_pair( 9999999, s ) );
		}
		//�ź����ǣ����������������Ǻܺ�
		1.����key���;���ĳ���ⲿ�ɼ�״̬�����״ֵ̬���Ա���������õ�
		  --����һ��ָ��ָ�򻺳�����ϵͳ�����������������key�Ϳ����޸�
		  ��һ���������ڼ��磬�Ǹ��ⲿ�ɼ���״̬������compare�ıȽϲ���
		  ��ô����ʹ��key����һ����֪����ʹ��ʹ���������ʽ�����Ĵ���һ
		  ����֪��������ⲿ�ɼ���״ֵ̬�����޸ģ����ǿ��Ըı��ֵ�����
		  ˳��
		2.����key������ΪString:compare���ͽ����key����Ϊ�ļ�����������
			��	ʱ���ȽϵĶ������ļ������ݡ���������£���ʹ����Ԫû�б��޸ģ�
			�����ļ�����һ�������޸ģ�������Դ����ǿ��Ա��޸�
			
			
	//����ͨ��set::iterator�޸�set�������� 
	
	//set::iterator��set::const_iterator�ǳ�������������Ҫ�޸�set
	//�Ķ��󣬱���ʹ��const_cast
	
	