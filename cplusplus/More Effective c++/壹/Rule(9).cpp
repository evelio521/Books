����:9
	/*����destructors������Դй©*/
	
	//
	class ALA{
		public:
			virtual void processAdoption()=0;
					
	};
	class Puppy:public:ALA{
		
		public:
			virtual void processAdoption();
		
	};
	class Kitten:public:ALA{
		
		public:
			virtual void processAdoption();
		
	};
	ALA *read(istream& s);//��s ��ȡ��Ϣ������һ��ָ�룬ֻ��һ���·���Ķ���
	void processAdoptions(istream& dataSource)
	{
		
		while(dataSource){//�����������
			ALA *pa=readALA(dataSource)//ȡ����һֻ����
			pa->processAdoption();//������������  �˴��������׳��쳣
			delete pa;//ɾ��readALA���صĶ���
		}
	}
	
	//�������2
	void processAdoptions(istream& dataSource)
	{
		
		while(dataSource){//�����������
			auto_ptr<ALA> pa=readALA(dataSource)//ȡ����һֻ����
			
			pa->processAdoption();//������������  
			//auto_ptr���Զ��ͷ���Դ
		}
	}
	//�������1(���Ǻܺ� ʹ�����ɬ ����ʹ�ý������2)
	void processAdoptions(istream& dataSource)
	{
		
		while(dataSource){//�����������
			ALA *pa=readALA(dataSource)//ȡ����һֻ����
			try{
			pa->processAdoption();//������������  
		}
		catch(...){
			delete pa;
			throw;
		}
			delete pa;
		}
	}
		