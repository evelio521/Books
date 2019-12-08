����:11
	 /*��ֹ�쳣��Ϣ��exceptions�����ݵ�����������*/
	 class Session {
		public:
			Session();
			~Session();
			...
		private:
			static void logCreation(Session *objAddr);
			static void logDestruction(Session *objAddr);
		};
		//����logCreation �� logDestruction���ֱ����ڼ�¼����Ľ������ͷš�������˿���������дSession������������
		Session::~Session()
		{
				logDestruction(this);
		}
		/*
		 *һ�п���ȥ�ܺã��������logDestruction�׳�һ���쳣���ᷢ��ʲô���أ�
		 *�쳣û�б�Session��������������ס�������������ݵ����������ĵ��������
		 *�������������������ĵ��þ���Դ����ĳЩ�����쳣���׳�����ôterminate
		 *���������Զ����ã�������ֹ��ĳ����ⲻ������ϣ�����������顣����û��
		 *��¼���ͷŶ������Ϣ�����ǲ��ҵģ�������һ�����鷳����ô��̬�������ص���
		 *������ֹ�������еĵز���ô�����û�У�������ֹ��logDestruction���׳���
		 *�쳣���ݵ�Session�������������档Ψһ�ķ�������try��catch blocks
		 */
		 
		 Session::~Session()
			{
				try {
					logDestruction(this);
				}
				catch (...) {
					cerr << "Unable to log destruction of Session object "
								<< "at address "
								<< this
								<< ".\n";
				}
			}
			
			/*
			 *������������������ԭ���Ĵ��밲ȫ�������catch�е���operator<<ʱ����
			 *һ���쳣���׳������Ǿ��������������⣬һ���쳣��ת�ݵ�Session�������������档
			 *���ǿ�����catch�з���try���������ܵ���һ���޶ȣ����������ѭ���������������
			 *��Sessionʱ������Ե��������׳����쳣��
			 */
			Session::~Session()
			{
			try {
			logDestruction(this);
			}
			catch (...) { }
			}
			/* catch�����Ϻ���û�����κ����飬����һ������ʵ��������ֹ���κδ�logDestruction
			 *�׳����쳣�����ݵ�session�������������档���������ܸ��������ˣ�����session�����ǲ�
			 *���ڶ�ջ��ջ��stack unwinding���б��ͷţ�terminate���������ᱻ���á�
			 */