ITEM34:Ԥ�����
	//����ʹ�ú������
	
	//1.�ػ�ͷ�ļ�
	
	//Ϊ�˷�ֹͷ�ļ���ΰ���������һ�ּ���
	#ifndef MYPROG_X_H
	#define MYPROG_X_H
	// ... the rest of the header file x.h goes here...
	#endif

  //2.ʹ��Ԥ��������
  //����ϴ����У������кŻ����ʱ��������Ϣͨ�������á�Ҫ������һ�㣬
  //һ���򵥵ķ�����ʹ��Ԥ�����׼�꣬��__FILE__, __LINE__, __DATE__, ��__TIME__. 
  //������ͬԭ���Լ�����ԭ��ʹ��stringizing(�ַ�����)��token-pasting(��Ǻϲ�)Ԥ���������Ҳ�����á�
  
  //3.�ڱ���ʱ��ѡ�����
    //A ���Դ���
      //�ڱ������ϵͳʱ����ʱ������ʹ��ĳЩ������룬����ʱ�����ֲ���������
       void f()
			{//���ʵ���������β�ͬ�Ĵ���
			#ifdef MY_DEBUG
			  cerr << "some trace logging" << endl;
			#endif
			  // ... the rest of f() goes here...
			}
			//���������ʽ�������#define�����
			void f()
		{
		  if( MY_DEBUG )
		  {
		    cerr << "some trace logging" << endl;
		  }
		  // ... the rest of f() goes here...
		}
		//B �ض�ƽ̨����
		//ͨ���ڴ�������ض�ƽ̨�Ĵ���ʱ���������factoryģʽ���������ַ������������ϻ������
		//����ʱ�ڻ����������ԡ�����ʱ�����ڴ��ڵĲ���̫�٣�����ѹ���һ�������factory����ʱ��
		//Ԥ������һ���л���ѡ����ķ���
		
		//C ��ͬ�����ݱ�ʾ��ʽ
		//һ��������������:����һ��ģ���������һ�������룬�ⲿ�û�������Ӧ����
		//һ���򵥵�enum,�����н��͡�����ģ���ڲ�������Ӧ�ñ�����һ��map�У����ڲ���
		// For outsiders
		//
		enum Error
		{
		  ERR_OK = 0,            // No error
		  ERR_INVALID_PARAM = 1, // <description>
		  ...
		};
		
		// For the module's internal use
		//
		map<Error,const char*> lookup;
		lookup.insert( make_pair( ERR_OK,
		                          (const char*)"No error" ) );
		lookup.insert( make_pair( ERR_INVALID_PARAM,
		                          (const char*)"<description>" ) );
		...
		//������ͬʱӵ�����ֱ�﷽ʽ������ϣ����ʵ����Ϣ�������Σ����˺���һħ��
		//���ǾͿ����������������򵥵�дһ�������б��ڱ���ʱ�ڴ�����Ӧ�����ݽṹ
		
		ERR_ENTRY( ERR_OK,            0, "No error" ),
		ERR_ENTRY( ERR_INVALID_PARAM, 1, "<description>" ),
		...

		
		//���׼��:������������⣬����ʹ��Ԥ�����
		  //1.�ػ�ͷ�ļ�
		  //2.�������룬�Ի�ȡ����ֲ�ԣ�����.cpp�ļ��н��в���
		  //3.��#pragma��ֹ���˴��ŵľ��棬��������#pragma�ܵð���һ��Ϊ�˻�ÿ���ֲ�Զ��ṩ��
		     //��������֮�У���ֹ����������ʶ���Ƕ���������
		
		
		
		
