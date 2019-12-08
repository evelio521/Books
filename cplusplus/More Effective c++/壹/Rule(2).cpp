����2:
	/*���ʹ��c++ת�Ͳ�����*/
	/*
	 *C++ͨ�������ĸ��µ�����ת���������˷���C�������ת����ȱ�㣬
	 *���ĸ���������, static_cast, const_cast, dynamic_cast, ��reinterpret_cast��
	 *�ڴ��������£�������Щ��������ֻ��Ҫ֪��ԭ����ϰ��������д��
   *         (type) expression
   *����������Ӧ������д��
   *          static_cast<type>(expression)
	 *
	 */
	 
	 /*
	  *static_cast�ڹ����ϻ�������C��������ת��һ��ǿ�󣬺���Ҳһ����
	  *��Ҳ�й��������ơ����磬�㲻����static_cast����C��������ת��
	  *һ����structת����int���ͻ��߰�double����ת����ָ�����ͣ�
	  *���⣬static_cast���ܴӱ��ʽ��ȥ��const���ԣ�
	  *��Ϊ��һ���µ�����ת��������const_cast�������Ĺ��ܡ�
	  */
	  int firstNumber, secondNumber;
		...
		double result = ((double)firstNumber)/secondNumber��
		//����������µ�����ת����������Ӧ������д��
		double result = static_cast<double>(firstNumber)/secondNumber;
		
		/*
		 *const_cast��������ת�������ʽ��const��volatileness���ԡ�
		 *ͨ��ʹ��const_cast���������Ǻͱ�����ǿ����ͨ������ת��������
		 *ֻ�Ǹı�һЩ������constness���� volatileness���ԡ�������屻��������Լ����
		 *�������ͼʹ��const_cast������޸�constness ����volatileness����֮������飬�������ת�������ܾ�
		 */
		 class Widget { ... };
		 class SpecialWidget: public Widget { ... };
		 void update(SpecialWidget *psw);
		 SpecialWidget sw; // sw ��һ����const ����
		 const SpecialWidget& csw = sw; // csw ��sw��һ������
		 																// ����һ��const ����
		 update(&csw); // ����!���ܴ���һ��const SpecialWidget* ����
		 							// ��һ������SpecialWidget*���ͱ����ĺ���
		 update(const_cast<SpecialWidget*>(&csw));
												// ��ȷ��csw��const����ʾ��ת������
												// csw��sw��������ֵ��update
												//�������ܱ����£�
		 update((SpecialWidget*)&csw);
												// ͬ�ϣ�������һ������ʶ��
												//��C��������ת��
		 Widget *pw = new SpecialWidget;
		 update(pw); // ����pw��������Widget*������
		 							// update�����������SpecialWidget*����
		 update(const_cast<SpecialWidget*>(pw));
									// ����const_cast���ܱ�����Ӱ��
                  // constness or volatileness�ĵط��ϡ�,
                  // ����������̳������������ת����
                  
     /*
      *�ڶ������������ת������dynamic_cast���������ڰ�ȫ��������ļ̳й�ϵ���½�������ת����
      *�����˵��������dynamic_cast��ָ������ָ�������ת����ָ��������������ֵ����ָ������ã�
      *��������֪��ת���Ƿ�ɹ���ʧ�ܵ�ת�������ؿ�ָ�루����ָ���������ת��ʱ��
      *�����׳��쳣���������ý�������ת��ʱ����
      */
      Widget *pw;
			...
			update(dynamic_cast<SpecialWidget*>(pw));
					// ��ȷ�����ݸ�update����һ��ָ��
					// ��ָ���������ΪSpecialWidget��pw��ָ��
					// ���pwȷʵָ��һ������,
					// ���򴫵ݹ�ȥ�Ľ�ʹ��ָ�롣
			void updateViaRef(SpecialWidget& rsw);
			updateViaRef(dynamic_cast<SpecialWidget&>(*pw));
					//��ȷ�� ���ݸ�updateViaRef����
					// SpecialWidget pw ָ�룬���pw
					// ȷʵָ����ĳ������
					// �����׳��쳣
			//dynamic_casts�ڰ���������̳в�����������Ƶġ������ܱ�����ȱ���麯���������ϣ��μ�����M24����
			//Ҳ����������ת����constness��
			int firstNumber, secondNumber;
			...
			double result = dynamic_cast<double>(firstNumber)/secondNumber;
							// ����û�м̳й�ϵ
			const SpecialWidget sw;
			...
			update(dynamic_cast<SpecialWidget*>(&sw));
			       // ����! dynamic_cast����ת����const��
			       
			/*
			 *���ĸ�����ת�����е����һ����reinterpret_cast��
			 *ʹ�����������������ת�������ת�������������ִ���ڶ��壨implementation-defined����
			 *��ˣ�ʹ��reinterpret_casts�Ĵ��������ֲ��
       *reinterpret_casts������ͨ����;�����ں���ָ������֮�����ת��
       */
       
      typedef void (*FuncPtr)(); // FuncPtr is һ��ָ����
																	// ��ָ�룬�ú���û�в���
																	// ����ֵ����Ϊvoid
			FuncPtr funcPtrArray[10]; // funcPtrArray ��һ��������
																// 10��FuncPtrsָ�������
		  int doSomething();
		  
		  funcPtrArray[0] = &doSomething; // �������Ͳ�ƥ��
      //reinterpret_cast����������ʹ����������ķ���ȥ�������ǣ�
      funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);// this compiles
      