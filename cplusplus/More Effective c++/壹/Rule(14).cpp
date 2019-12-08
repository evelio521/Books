����:14
	/*���Ƕ�������ʹ��exception specification*/
	
	/*
	 *���ҵ��ǣ����Ǻ����׾��ܹ���д�����·����������ѵĺ������������������ֵؼ���쳣
	 *��ʹ���Ƿ����쳣��񱣳�һ�¡�һ��������������һ�����������Һ��߿����׳�һ��Υ��
	 *ǰ���쳣�����쳣����A��������B����������ΪB���������׳�һ������A�����쳣���֮
	 *�ڵ��쳣����������������þ�Υ����A�������쳣��� ����ע�����������Դ����������
	 *��⣬�������Ա�׼Ҳ��ֹ�������ܾ����ֵ��÷�ʽ.
	 */
	 	extern void f1(); // �����׳�������쳣
		//������һ������f2ͨ�������쳣�����������ֻ���׳�int���͵��쳣��
		void f2() throw(int);
		//f2����f1�Ƿǳ��Ϸ��ģ���ʹf1�����׳�һ��Υ��f2�쳣�����쳣��
		void f2() throw(int)
		{
		...
				f1(); // ��ʹf1�����׳�����int���͵�
				//�쳣����Ҳ�ǺϷ��ġ�
				...
		}
		
		//ʹ��exception specification������ѡ��
		1.���⽫exception specification������Ҫ�����Ա�����template����
		  // һ��������template��� ��Ϊ������exception specification
			template<class T>
			bool operator==(const T& lhs, const T& rhs) throw()
			{
					return &lhs == &rhs;
			}
			
			/* 
			 *���ģ��������쳣����ʾģ�����ɵĺ��������׳��쳣��������ʵ���ܲ�����������
			 *Ϊopertor&(��ַ������,�μ�Effective C++ ����45)�ܱ�һЩ���Ͷ������ء��������
			 *�صĻ��������ô�operator==�����ڲ�����opertor&ʱ��opertor&���ܻ��׳�һ���쳣��
			 *������Υ�������ǵ��쳣���ʹ�ó��������ת��unexpected��������������һ�ָ�һ
			 *������������������һ������Ҳ����û�а취֪��ĳ��ģ�����Ͳ����׳�ʲô�����쳣��
			 *���Ǽ���������Ϊһ��ģ���ṩһ����������쳣�����Ϊģ�����ǲ��ò�ͬ�ķ���ʹ
			 *�����Ͳ������������ֻ����ģ����쳣���Ҫ���ʹ�á�
			 */
		2.���A�����ڵ�����B��������B������exception specification����ôA��������Ҳ��Ҫ�趨exception specification��
		  // һ��windowϵͳ�ص�����ָ��
			//��һ��windowϵͳ�¼�����ʱ
			typedef void (*CallBackPtr)(int eventXLocation,
			int eventYLocation,
			void *dataToPassBack);
			//windowϵͳ�࣬���лص�����ָ�룬
			//�ûص������ܱ�windowϵͳ�ͻ�ע��
			class CallBack {
					public:
						CallBack(CallBackPtr fPtr, void *dataToPassBack): func(fPtr), data(dataToPassBack) {}
						void makeCallBack(int eventXLocation,
						int eventYLocation) const throw();
				private:
						CallBackPtr func; // function to call when
						// callback is made
						void *data; // data to pass to callback
			}; // function
			// Ϊ��ʵ�ֻص����������ǵ���ע�ắ����
			//�¼���������ע��������Ϊ����������
			void CallBack::makeCallBack(int eventXLocation,
			int eventYLocation) const throw()
			{
					func(eventXLocation, eventYLocation, data);
			}
			//������makeCallBack�ڵ���func��ҪðΥ���쳣���ķ��գ���Ϊ�޷�֪��func���׳�ʲô���͵��쳣��
			//ͨ���ڳ�����CallBackPtr typedef�в��ø��ϸ���쳣�����������⣺
			typedef void (*CallBackPtr)(int eventXLocation,int eventYLocation,void *dataToPassBack) throw();
			
			//��������typedef�����ע��һ�����ܻ��׳��쳣��callback�������ǷǷ��ģ�
				// һ��û���쳣���Ļص�����
				void callBackFcn1(int eventXLocation, int eventYLocation,
				void *dataToPassBack);
				void *callBackData;
				...
				CallBack c1(callBackFcn1, callBackData);
				//����callBackFcn1����
				// �׳��쳣
				//�����쳣���Ļص�����
				void callBackFcn2(int eventXLocation,
				int eventYLocation,
				void *dataToPassBack) throw();
				CallBack c2(callBackFcn2, callBackData);
				// ��ȷ��callBackFcn2
				// û���쳣���
			3.�Ǵ���ϵͳ�����׳���exception����Щ�쳣���������bad_alloc�����ڴ����ʧ��ʱ����
				operator new ��operator new[]�׳����μ�����M8����������ں�����ʹ��new�����������μ�����M8����
				�����Ϊ������������bad_alloc�쳣����׼����
				
		
		
		//����취��
		//��Ȼ��ֹ�׳�unexpected�쳣�ǲ���ʵ�ģ�����C++��������������ͬ���쳣�����滻unexpected�쳣��
		//���ܹ�����������ԡ�������ϣ�����е�unexpected�쳣�����滻ΪUnexpectedException��������������д���룺
			class UnexpectedException {}; // ���е�unexpected�쳣����
			//�滻Ϊ�������Ͷ���
			void convertUnexpected() // ���һ��unexpected�쳣��
			{ // �׳����������������
					throw UnexpectedException();
			}
			//ͨ����convertUnexpected�����滻ȱʡ��unexpected��������ʹ�������뿪ʼ���С���
			set_unexpected(convertUnexpected);
			
			//������ô�����Ժ�һ��unexpected�쳣����������convertUnexpected������Unexpected�쳣��
			//һ��UnexpectedException���쳣�����滻�������Υ�����쳣������UnexpectedException�쳣��
			//��ô�쳣���ݽ�������ȥ�������쳣������ǵõ����㡣������쳣���û�а���UnexpectedException��
			//terminate�������ã��ͺ�����û���滻unexpectedһ����
			
			//��һ�ְ�unexpected�쳣ת���֪�����͵ķ������滻unexpected���������������׳���ǰ�쳣��
			//�����쳣�����滻Ϊbad_exception�������������д��
				void convertUnexpected() // ���һ��unexpected�쳣��
				{ //�׳����������������
				throw; // ��ֻ�������׳���ǰ
				} // �쳣
				set_unexpected(convertUnexpected);
				// ��װ convertUnexpected
				// ��Ϊunexpected
				// �����Ʒ
			
			//�����ô������Ӧ�������е��쳣��������bad_exception�������Ļ��࣬��׼��exception����
			//�㽫�����ٵ����������unexpected�쳣�ᵼ�³���������ֹ���κβ��������쳣�������滻Ϊ
			//bad_exception������쳣����ԭ�����쳣�������ݡ�
			
			
			class Session { // for modeling online
			public: // sessions
				~Session();
				...
			private:
				static void logDestruction(Session *objAddr) throw();
			};
			Session::~Session()
			{
				try {
					logDestruction(this);
				}
				catch (...) { }
			}
			
			/*
			 *session��������������logDestruction��¼�й�session�����ͷŵ���Ϣ������ȷ��Ҫ�����
			 *logDestruction�׳��������쳣������logDestruction���쳣����ʾ�䲻�׳��κ��쳣������
			 *���豻logDestruction���õĺ����׳���һ���쳣����logDestructionû�в������ǲ���������
			 *�����������飬���������������������׾ͻ�д��Υ���쳣���Ĵ��롣������쳣ͨ��logDestruction
			 *���ݳ�����unexpected�������ã�ȱʡ����½����³�����ִֹ�С�����һ����ȷ����Ϊ��������session
			 *����������������ϣ������Ϊô�������봦�����п��ܵ��쳣�����Ժ���Ӧ�ò���session�����������
			 *catch��ִ�еĻ������ֹ�������logDestructionû���쳣�����������Ͳ��ᷢ����һ�ַ�ֹ�ķ�
			 *���������������������滻unexpected����
			 */