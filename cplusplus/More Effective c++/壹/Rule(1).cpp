����1:
	/*��ϸ����pointers��reference*/
	/*
	 * û����ν��null reference һ��reference�������һ������
	 * pointer������Ϊnull
	 * 
	 */
	 
	 //����Ĵ������к��� ���ܵ��²���Ԥ�ڵ���Ϊ
	  char *pc=0;
	  char& rc=*pc;
	  
	 //c++Ҫ�� reference�����г�ֵ
	 string& rs;//����
	 string s("xyzxyz");
	 string& rs=s;//ok
	 
	 //pointer����û�г�ֵ
	 string* ps;
	 
	 //reference��pointer����Ч�� ʹ��ǰ����Ҫ������Ч��
	 void printDouble(const double& rd)
	   {
	   	cout<<rd;
	  	}
	  void printDouble(const double* rd)
	   {
	   	if(rd)
	   	{
	   	cout<<*rd;
	  	}
	  }
	  
	  //pointer���Ա����¸�ֵ����referenceȴ����ָ�������ֵ�Ķ���
	  
	  string s1("nacy");
	  string s2("clancy");
	  
	  string& rs=s1;
	  string *ps=&s1;
	  rs=s2;
	  
	  ps=&s2;
	  
	  /*
	   *����һ����������ǵ�������ĳ��������ʱ����Ӧ��ʹ�����á�
	   *����ͨ�������ǲ�����[]��������������͵��÷��Ƿ���һ��Ŀ��������ܱ���ֵ��
	   */
		vector<int> v(10); // ��������������vector������СΪ10;
											// ������һ���ڱ�׼C���е�һ��ģ��(������M35)
		v[5] = 10;     // �������ֵ��Ŀ�������ǲ�����[]���ص�ֵ
		
		//���������[]����һ��ָ�룬��ô��һ�����͵�����д��
		
		*v[5] = 10;
	  
	 