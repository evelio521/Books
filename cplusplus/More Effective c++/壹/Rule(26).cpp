����26:
	/*����ĳ�������ܲ����Ķ�������*/
	//��ֹ��������
	class CantBeInstantiated {
		private:
			CantBeInstantiated();
			CantBeInstantiated(const CantBeInstantiated&);
			...
	};
	
	//ֻ�ܲ���һ������
	class PrintJob; // forward ����
		// �μ�Effective C++����34
	class Printer {
		public:
			void submitJob(const PrintJob& job);
			void reset();
			void performSelfTest();
			...
			friend Printer& thePrinter();
		private:
			Printer();
			Printer(const Printer& rhs);
			...
	};
	Printer& thePrinter()
	{
		static Printer p; // ������ӡ������
		return p;
	}
	
	/*
	 * ������������������ɣ���һ��Printer��Ĺ��캯����private��
	 * ��������ֹ�������󡣵ڶ���ȫ�ֺ���thePrinter������Ϊ�����Ԫ��
	 * ��thePrinter����˽�й��캯����������ơ����thePrinter����һ��
	 * ��̬Printer��������ζ��ֻ��һ�����󱻽�����
	 */
	 
	 class PrintJob {
			public:
				PrintJob(const string& whatToPrint);
				...
		};
		string buffer;
				... //���buffer
		thePrinter().reset();
		thePrinter().submitJob(buffer);
		
		//��һ�ֲ���һ������ķ���
		class Printer {
			public:
				static Printer& thePrinter();
				...
			private:
				Printer();
				Printer(const Printer& rhs);
				...
		};
		Printer& Printer::thePrinter()
		{
			static Printer p;
			return p;
		}
		//�û�ʹ��printerʱ��Щ������
		Printer::thePrinter().reset();
		Printer::thePrinter().submitJob(buffer);
			
		//����namespace��
		namespace PrintingStuff {
			class Printer { // �������ռ�
				public: // PrintingStuff�е���
					void submitJob(const PrintJob& job);
					void reset();
					void performSelfTest();
					...
					friend Printer& thePrinter();
			private:
					Printer();
					Printer(const Printer& rhs);
					...
		};
	Printer& thePrinter() // �������Ҳ�������ռ���
		{//����ʹ��inline ������̲�
			static Printer p;
			return p;
		}
}// �����ռ䵽�˽���

	//ʹ����������ռ�󣬿ͻ��˿���ͨ��ʹ��fully-qualified name����ȫ���Ʒ������������������ռ�����֣���
			PrintingStuff::thePrinter().reset();
			PrintingStuff::thePrinter().submitJob(buffer);
	//����Ҳ����ʹ��using�������Լ򻯼������룺
			using PrintingStuff::thePrinter; // �������ռ�"PrintingStuff"
															//��������"thePrinter"
															// ʹ���Ϊ��ǰ��
			thePrinter().reset(); // ���ڿ�����ʹ�þֲ�����
			thePrinter().submitJob(buffer); // һ����ʹ��thePrinter
			
			//����һ���������һ��д��
			class Printer {
					public:
						class TooManyObjects{}; // ����Ҫ�Ķ������ʱ
																		// ��ʹ������쳣��
						Printer();
						~Printer();
						...
					private:
						static size_t numObjects;
						Printer(const Printer& rhs); // ����ֻ����һ��printer��
																				// ���Բ�������
			}; // ���μ�Effective C++ ����27��
		//�˷��ĺ���˼�����ʹ��numObjects����Pritner������ڵ���������������ʱ������ֵ�����ӣ��ͷ���ʱ������ֵ�ͼ��١������ͼ��������Printer���󣬾ͻ��׳�һ��TooManyObjects���͵��쳣��
		// Obligatory definition of the class static
			size_t Printer::numObjects = 0;
			Printer::Printer()
			{
				if (numObjects >= 1) {
				 throw TooManyObjects();
			}
		//�������������Ĺ��캯��;
			++numObjects;
			}
			Printer::~Printer()
			{
			//����������������������;
				--numObjects;
			}
			
			//������Ĵ���
			class ColorPrinter: public Printer {
					...
			};
			//���ڼ�������ϵͳ��һ����ͨ��ӡ����һ����ɫ��ӡ����
			Printer p;
			ColorPrinter cp;
			//�������������Printer����ʱ���ᷢ��ͬ�������⣺
			class CPFMachine { // һ�ֻ��������Ը�ӡ����ӡ
				private: // �����档
					Printer p; // �д�ӡ����
					FaxMachine f; // �д�������
					CopyMachine c; // �и�ӡ����
					...
			};
		CPFMachine m1; // ��������
		CPFMachine m2; // �׳� TooManyObjects�쳣
		
		//������һ�������������� ���ǲ�ϣ�����̳�
		
		class FSA {
			public:
				// α���캯��
				static FSA * makeFSA();
				static FSA * makeFSA(const FSA& rhs);
				...
			private:
				FSA();
				FSA(const FSA& rhs);
				...
	};
		FSA * FSA::makeFSA()
			{ return new FSA(); }
		FSA * FSA::makeFSA(const FSA& rhs)
			{ return new FSA(rhs); }
			
			//Ϊ�˷�ֹ�ڴ�й© �������µ���
			auto_ptr<FSA> pfsa1(FSA::makeFSA());
			// indirectly call FSA copy constructor
			auto_ptr<FSA> pfsa2(FSA::makeFSA(*pfsa1));
			... // ��ͨ����ָ��һ��ʹ��pfsa1��pfsa2,
					//�������ò���ɾ�����ǡ�
					
			
			//�����ض�������� ���10��
			class Printer {
				public:
					class TooManyObjects{};
					// α���캯��
					static Printer * makePrinter();
					static Printer * makePrinter(const Printer& rhs);
					...
				private:
					static size_t numObjects;
					static const size_t maxObjects = 10; // ���������
					Printer();
					Printer(const Printer& rhs);
		};
			// Obligatory definitions of class statics
			size_t Printer::numObjects = 0;
			const size_t Printer::maxObjects;
			Printer::Printer()
			{
				if (numObjects >= maxObjects) {
					throw TooManyObjects();
			}
			...
			}
			Printer::Printer(const Printer& rhs)
			{
				if (numObjects >= maxObjects) {
					throw TooManyObjects();
			}
				...
			}
			Printer * Printer::makePrinter()
				{ return new Printer; }
			Printer * Printer::makePrinter(const Printer& rhs)
				{ return new Printer(rhs); }
			//maxObjectҲ������������
			class Printer {
				private:
					enum { maxObjects = 10 }; // ������,
					... // maxObjectsΪ����10
			};
		  
		  
		  //һ������������������Base Class
		  template<class BeingCounted>
			class Counted {
				public:
					class TooManyObjects{}; // �����׳��쳣
					static int objectCount() { return numObjects; }
				protected:
					Counted();
					Counted(const Counted& rhs);
					~Counted() { --numObjects; }
				private:
					static int numObjects;
					static const size_t maxObjects;
					void init(); // ���⹹�캯����
			}; // �����ظ�
		template<class BeingCounted> // ����numObjects
		int Counted<BeingCounted>::numObjects; // �Զ�������ʼ��Ϊ0
			
		template<class BeingCounted>
		Counted<BeingCounted>::Counted()
			{ init(); }
			
		template<class BeingCounted>
		Counted<BeingCounted>::Counted(const Counted<BeingCounted>&)
			{ init(); }
			
		template<class BeingCounted>
		void Counted<BeingCounted>::init()
			{
				if (numObjects >= maxObjects) throw TooManyObjects();
				++numObjects;
			}

	//Printer��д��
		class Printer: private Counted<Printer> {
			public:
			// α���캯��
			static Printer * makePrinter();
			static Printer * makePrinter(const Printer& rhs);
			~Printer();
			void submitJob(const PrintJob& job);
			void reset();
			void performSelfTest();
			...
			using Counted<Printer>::objectCount; // �μ��������
			using Counted<Printer>::TooManyObjects; // �μ��������
		private:
			Printer();
			Printer(const Printer& rhs);
		};
		
		//����maxObject����ѡ�����û�������
		//Printer�����߱�������������뵽һ��ʵ���ļ��
		const size_t Counted<Printer>::maxObjects = 10;