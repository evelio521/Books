ITEM9:��ͬ�Ĵ�����
	//1.Describe what the following code does:
	
	// Example 9-1
	//
	f( a++ );
	
	//Be as complete as you can about all possibilities
	
	//���
	//��f�������һ��
	    #define f(x) x                      // once
			#define f(x) (x,x,x,x,x,x,x,x,x)    // 9 times
			#define f(x)                        // not at all
   //���׼�� ����ʹ�ú꣬������ʹ�ô����������⣬�Ӷ�����ά��

	//���Ǻ���
	   
	   //��������£����ȣ�a++�ᱻ��ֵ����������ݸ�������������ͨ��
	   //���ö��������ʱ�������ʽ����a �ľ�ֵ������f��ȡ�����ķ�ʽ
	   //Ҫôͨ����ֵ��Ҫôͨ��const���ã��������Դ��ݷ�const���ã�
	   //��Ϊ��const���ò��ܰ�����ʱ����
	 //���Ƕ���
	 
	   //f��һ���������������ڷº�����f��operator()()�����Դ�ֵ��const
	   //���û�ȡ����
	//������������
	
	   //�ȶ�a++��ֵ������f����ʱ����
	   
  //��a�Ǻ�
     //���������a���Ա�ʾ�κζ���
  //��a�Ƕ���(�п������ڽ�����)
     //��������±��붨����ʵĺ��õ���������
     // Canonical form of postincrement:
			T T::operator++(int)
			{
			  T old( *this ); // remember our original value
			
			  ++*this;        // always implement postincrement
			                  //  in terms of preincrement
			
			  return old;     // return our original value
			}
			
			//���׼��:ʼ��Ϊ���ص������������ȷ������
  //2.�������δ��������

		// Example 9-2(a)
		//
		f( a++ );
		
		//(1) a++ ����a������a�ľ�ֵ
		//(2)f()  ��a�ľ�ֵ���ݸ�f(),ִ��f()
		// Example 9-2(b)
		//
		f( a );
		a++;
		//(1) f() ��a�ľ�ֵ���ݸ�f(),Ȼ��ִ��f()
		//(2)a++ ����a������a�ľ�ֵ,��ֵ��󱻺���
		
		
		//����������Ҫ���
		//(1) f()�����쳣������£���һ�����뱣֤a++�������еĸ����ö��ɹ�ִ�н��������ڶ������뱣֤a++û��ִ�У����ĸ�����һ��Ҳû����
		//(2) ��ʹû���쳣���������f()��a.operator++(int)�пɼ��ĸ����ã���ô���ǵ�ִ��˳�����Ҫ��
		
		//3.����f()��һ����������ͨ����ֵ�ķ�ʽ��ò�����a��һ��������ṩ�����������operator++(int)
		   //��ô����1�ʹ���2������������
		 //������ȫ�����c++���ڶ��δ��벻�Ϸ�����һ�δ���Ϸ�
		 
		 //�������f()����list::erase()
		 //��һ����ʽ�Ϸ�
		 // Example 9-3(a)
			//
			// l is a list<int>
			// i is a valid non-end iterator into l
			//
			l.erase( i++ ); // OK, incrementing a valid iterator
			
			//�ڶ��δ��벻�Ϸ�:
			
			// Example 9-3(b)
			//
			// l is a list<int>
			// i is a valid non-end iterator into l
			//
			l.erase( i );
			i++;            // error, i is not a valid iterator
			
