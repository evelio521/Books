ITEM6:��ʱ����
	//�ҳ�������ʱ����ʹ�õĵط�
	string FindAddr( list<Employee> emps, string name ) //�����������������ʱ����
		{
		  for( list<Employee>::iterator i = emps.begin();
		       i != emps.end();
		       i++ )//��ʱ�������
		  {
		    if( *i == name )//��ʱ�������
		    {
		      return i->addr;
		    }
		  }
		  return "";//��ʱ�������
		}
	
	  //ע�����¼���
	  1.��ʹ��const&�������Ǵ�ֵ����
	  2.��ʹ��++i ����ʹ��i++
	  3.ʱ��ע����Ϊ����ת�����������������ص���ʱ����
	    һ���������õķ�����ʹ����ʾ�Ĺ��캯��
	  4.���Ծ��Բ�Ҫ���ؾ������������

  //�������������Ż�
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

