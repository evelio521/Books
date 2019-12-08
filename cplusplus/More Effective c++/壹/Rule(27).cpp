 ����27:
 	/*��Ҫ����ֹ�ڶ��в�������*/
 	/*
 	 * Ҫ���ڶ��н�������
   * �������ȴӱ����ڶ��н�������ʼ˵��Ϊ��ִ���������ƣ�
   * ������ҵ�һ�ַ�����ֹ�Ե��á�new������������ֶν�������
   * ��������������ǶѶ���non-heap object���ڶ������ĵط���
   * �Զ����죬������ʱ�����ʱ�Զ����ͷţ�����ֻҪ��ֹʹ����ʽ
   * �Ĺ��캯���������������Ϳ���ʵ���������ơ�
   */
   class UPNumber {
			public:
				UPNumber();
				UPNumber(int initValue);
				UPNumber(double initValue);
				UPNumber(const UPNumber& rhs);
					// α�������� (һ��const ��Ա������ ��Ϊ
					// ��ʹ��const����Ҳ�ܱ��ͷš�)
				void destroy() const { delete this; }
				...
			private:
				~UPNumber();
	};
		//Ȼ��ͻ����������г�����ƣ�
		UPNumber n; // ����! (������Ϸ��� ����
								// ������������������ʽ��
								// ����ʱ���Ͳ��Ϸ���)
		UPNumber *p = new UPNumber; //��ȷ
		...
		delete p; // ����! ��ͼ����
		// private ��������
		p->destroy(); // ��ȷ
		
		/*
		 * ��һ�ַ����ǰ�ȫ���Ĺ��캯��������Ϊprivate�����ַ�����ȱ��
		 * ��һ���ྭ������๹�캯����������߱����ס�����Ƕ�����Ϊ
		 * private�����������Щ�����ͻ��ɱ��������ɣ����캯����������
		 * ���캯����Ҳ����ȱʡ���캯�������������ɵĺ�������public
		 *���μ�Effecitve C++ ����45������˽���������������Ϊprivate
		 * �Ǻܼ򵥵ģ���Ϊÿ����ֻ��һ������������
		 */
		 
		 
		 
		 
		 
		 /*
		  * ͨ�����Ʒ���һ������������������Ĺ��캯������ֹ�����ǶѶ���
		  * ����������26�Ѿ�˵�������ַ���Ҳ��ֹ�˼̳кͰ��ݣ�containment��
		  * ��Щ���Ѳ��ǲ��ܿ˷��ġ�ͨ����UPNumber��������������Ϊprotected
		  *��ͬʱ���Ĺ��캯��������public���Ϳ��Խ���̳е����⣬��Ҫ����UPNumber
		  * �����������޸�Ϊ����ָ��UPNumber��ָ��
		  */
		  class UPNumber { ... }; // ������������Ϊprotected
			class NonNegativeUPNumber:
				public UPNumber { ... }; // ������ȷ��; ������ �ܹ����� protected ��Ա
			class Asset {
				public:
					Asset(int initValue);
					~Asset();
					...
				private:
					UPNumber *value;
		};
		Asset::Asset(int initValue): value(new UPNumber(initValue)) // ��ȷ
			{ ... }
		Asset::~Asset()
			{ value->destroy(); } // Ҳ��ȷ
			
			/*�ж�һ�������Ƿ��ڶ���*/
			class HeapTracked { // �����; ����
				public: // ��operator new���ص�ptr
					class MissingAddress{}; // �쳣�࣬���������
					virtual ~HeapTracked() = 0;
					static void *operator new(size_t size);
					static void operator delete(void *ptr);
					bool isOnHeap() const;
					private:
					typedef const void* RawAddress;
					static list<RawAddress> addresses;
		};

		// mandatory definition of static class member
		list<RawAddress> HeapTracked::addresses;
		// HeapTracked�����������Ǵ��麯����ʹ�ø����Ϊ�����ࡣ
		// (�μ�Effective C++����14). Ȼ�������������뱻���壬
		//������������һ���ն��塣.
		HeapTracked::~HeapTracked() {}
		void * HeapTracked::operator new(size_t size)
		{
			void *memPtr = ::operator new(size); // ����ڴ�
			addresses.push_front(memPtr); // �ѵ�ַ�ŵ�list��ǰ��
			return memPtr;
		}
		void HeapTracked::operator delete(void *ptr)
		{
			//�õ�һ�� "iterator"������ʶ��listԪ�ذ�����ptr��
			//�й�ϸ�ڲμ�����35
			list<RawAddress>::iterator it =
			find(addresses.begin(), addresses.end(), ptr);
			if (it != addresses.end()) { // �������һ��Ԫ��
				addresses.erase(it); //��ɾ����Ԫ��
				::operator delete(ptr); // �ͷ��ڴ�
			} else { // ����
					throw MissingAddress(); // ptr�Ͳ�����operator new
			} // ����ģ������׳�һ���쳣
			}
		bool HeapTracked::isOnHeap() const
		{
			// �õ�һ��ָ�룬ָ��*thisռ�ݵ��ڴ�ռ����ʼ����
			// �й�ϸ�ڲμ����������
			const void *rawAddress = dynamic_cast<const void*>(this);
			// ��operator new���صĵ�ַlist�в鵽ָ��
			list<RawAddress>::iterator it =
			find(addresses.begin(), addresses.end(), rawAddress);
			return it != addresses.end(); // ����it�Ƿ��ҵ�
		}
		
		//ʹ������࣬��ʹ��������ĳ���ԱҲ���������м�����ٶ���ָ��Ĺ��ܡ�
		//��������Ҫ���ľ��������ǵ����HeapTracked�̳������������������ж�
		//Assert����ָ��ָ����Ƿ��ǶѶ���
		class Asset: public HeapTracked {
				private:
				UPNumber value;
				...
		};
		//�����ܹ�������ѯAssert*ָ�룬������ʾ��
			void inventoryAsset(const Asset *ap)
				{
					if (ap->isOnHeap()) {
				ap is a heap-based asset �� inventory it as such;
					}
					else {
				ap is a non-heap-based asset �� record it that way;
					}
				}
		//��HeapTracked�����Ļ������һ��ȱ�㣬�����������ڽ����ͣ�
		//��Ϊ��int��char���������Ͳ��ܼ̳����������͡�����ʹ����
		//HeapTracked��ԭ��һ�㶼��Ҫ�ж��Ƿ���Ե��á�delete this����
		//�㲻�������ڽ������ϵ���������Ϊ�ڽ�����û��thisָ�롣
		
		/*��ֹ�Ѷ���*/
		
		//��ֹ�û�ֱ��ʵ��������ܼ򵥣���Ϊ���ǵ���new���������ֶ���
		//���ܹ���ֹ�û�����new���㲻��Ӱ��new�������Ŀ����ԣ�������Ƕ�����Եģ���
		//�������ܹ�����new���������ǵ���operator new������㣨�μ�����M8����
		//���ﵽĿ�ġ�������Լ����������������������԰�������Ϊprivate��
		//���磬������벻�����û��ڶ��н���UPNumber���������������д��
		class UPNumber {
				private:
					static void *operator new(size_t size);
					static void operator delete(void *ptr);
...
};
		//�����û��������������������������飺
		UPNumber n1; // okay
		static UPNumber n2; // also okay
		UPNumber *p = new UPNumber; // error! attempt to call
		// private operator new
		//��operator new����Ϊprivate���㹻�ˣ����ǰ�operator new����Ϊprivate��
		//����iperator delete����Ϊpublic����������Щ���죬���Գ����о�����Ҫ��ԭ��
		//����Ҫ�����Ƿֿ���������������һ���������������ǡ������Ҳ���ֹ
		//UPNumber�Ѷ������飬���԰�operator new[]��operator delete[]���μ�����M8��Ҳ����Ϊprivate��
		
		
		//��operator new����Ϊprivate�������谭UPNumber������Ϊһ��λ�ڶ�
		//�е����������Ļ��౻ʵ��������Ϊoperator new��operator delete��
		//�Զ��̳еģ����operator new��operator deleteû�����������б�����
		//Ϊpublic�����и�д��overwrite�������Ǿͻ�̳л�����private�İ汾��������ʾ��
		class UPNumber { ... }; // ͬ��
		class NonNegativeUPNumber: //���������
			public UPNumber { //û������operator new
			...
		};
		NonNegativeUPNumber n1; // ��ȷ
		static NonNegativeUPNumber n2; // Ҳ��ȷ
		NonNegativeUPNumber *p =new NonNegativeUPNumber; // ����! ��ͼ����
 																					// private operator new
 																					
 		//UPNumber��operator new��private��һ�㣬����԰���UPNumber��Ա����Ķ���ķ�������κ�Ӱ�죺
			class Asset {
				public:
					Asset(int initValue);
					...
				private:
					UPNumber value;
		};
		Asset *pa = new Asset(100); // ��ȷ, ����
		// Asset::operator new ��
		// ::operator new, ����
		// UPNumber::operator new