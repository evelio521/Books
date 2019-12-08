ITEM19:���븴����
	//�������к������쳣��ȫ�Ļ���һ��������
	String EvaluateSalaryAndReturnName( Employee e ) 
{
  if( e.Title() == "CEO" || e.Salary() > 100000 )
  {
    cout << e.First() << " " << e.Last() << " is overpaid" << endl;
  }
  return e.First() + " " + e.Last();
}

 //basic guarantee�Ǳ�֤��������û��й©
 //strong guarantee�Ǳ�֤��������û��й©�⻹��֤commit-to-rollback
 
 //�����ܲ������ɻָ������ã��������<<�����stringһ���ֺ��׳��쳣��
 //��������Ǹ������ǲ��ܷ�����ģ���Ȼ���Ĵ���״̬Ҳ���ܱ����á�
 //����������ʱ�����������
 
 
 //���ϵĴ�������basic guarantee
 //��һ���޸�
 // Attempt #1: An improvement? 
	//
	String EvaluateSalaryAndReturnName( Employee e )
	{
	  String result = e.First() + " " + e.Last();
	  if( e.Title() == "CEO" || e.Salary() > 100000 )
	  {
	    String message = result + " is overpaid\n";
	    cout << message;
	  }
	
	  return result;
	}
	
  //�������д���
  String theName;
  theName=EvaluateSalaryAndReturnName(bob);
  //�������õ�by value�ķ�ʽ���أ���˻���copy assignment operator��
  //�������캯�������������һ��ʧ�ܣ���ô�����þͻ����
  //Ϊ�˱��⿽�����죬������һ�ΰ��޸�
  // Attempt #2: Better now? 
		//
		void EvaluateSalaryAndReturnName( Employee e,
		                                  String&  r )
		{
		  String result = e.First() + " " + e.Last();
		  if( e.Title() == "CEO" || e.Salary() > 100000 )
		  {
		    String message = result + " is overpaid\n";
		    cout << message;
		  }
		
		  r = result;
		}
 //����r�ĸ�ֵ����ʧ�ܣ�����������γ���
 // Attempt #3: Correct (finally!). 
	//
	auto_ptr<String>
	EvaluateSalaryAndReturnName( Employee e )
	{
	  auto_ptr<String> result
	      = new String( e.First() + " " + e.Last() );
	
	  if( e.Title() == "CEO" || e.Salary() > 100000 )
	  {
	    String message = (*result) + " is overpaid\n";
	    cout << message;
	  }
	
	  return result;  // rely on transfer of ownership;
	                  // this can't throw
	}
	//ֻҪ��ӡ������׳��쳣 �����Ǿ��ܰѺ��������ȫ�ķ��ظ�������
	
   //�յ�
   1.Ҫ���쳣��ȫ���ṩ��֤��������Ҫ����һ��������Ϊ����
   2.���һ�������ж��ظ����ã���ô�޷���Ϊǿ�쳣��ȫ������취���ǲ�ֳɼ�������
   3.���������еĺ�������Ҫ�쳣��ȫ�������е�һ�γ����Ѿ��㹻���ã�������Ҫ
     ������������ʧһ�������ܡ�