����4:
	/*�Ǳ�Ҫ��Ҫ�ṩdefault constructor*/
	/*
	 * ��һ������������������κ����ݼ��ɽ��������
	 * ����԰���ȱʡ���캯��������Ҫ�����������������
	 * ���ܰ���ȱʡ���캯��
	 */
	 
	 class EquipmentPiece {
		public:
			EquipmentPiece(int IDNumber);
		...
		};
		
		EquipmentPiece bestPieces[10]; // ����û����ȷ����
                                   // EquipmentPiece ���캯��
    EquipmentPiece *bestPieces =new EquipmentPiece[10];
                                  // ���������������һ��
    
    //�������
    int ID1, ID2, ID3, ..., ID10; // �洢�豸ID�ŵ�
                                 // ����
		...
		EquipmentPiece bestPieces[] = { // ��ȷ, �ṩ�˹���
		EquipmentPiece(ID1), // �����Ĳ���
		EquipmentPiece(ID2),
		EquipmentPiece(ID3),
		...,
		EquipmentPiece(ID10)
		};
		
		
		//һ����ͨ�õĽ������������ָ������������һ����������
		typedef EquipmentPiece* PEP; // PEP ָ��ָ��
                                 //һ��EquipmentPiece����
    PEP bestPieces[10]; // ��ȷ, û�е��ù��캯��
		PEP *bestPieces = new PEP[10]; // Ҳ��ȷ
		//��ָ���������ÿһ��ָ�뱻���¸�ֵ����ָ��һ����ͬ��EquipmentPiece����
		//�������з���������ȱ�㣬��һ�����ɾ��������ÿ��ָ����ָ��Ķ���
		//��������ˣ��ͻᷢ���ڴ�й©���ڶ��������ڴ����������Ϊ��������
		//Ҫ�ռ�������EquipmentPiece����һ������Ҳ��Ҫ�ռ�������ָ�롣
		for (int i = 0; i < 10; ++i)
				bestPieces[i] = new EquipmentPiece( ID Number );
				
		
		//�����Ϊ�������raw memory����Ϳ��Ա����˷��ڴ档
		// Ϊ��СΪ10������ �����㹻���ڴ�
		// EquipmentPiece ����; ��ϸ�����μ�����M8
		// operator new[] ����
		void *rawMemory =operator new[](10*sizeof(EquipmentPiece));
		// make bestPieces point to it so it can be treated as an
		// EquipmentPiece array
		EquipmentPiece *bestPieces =
		static_cast<EquipmentPiece*>(rawMemory);
		// construct the EquipmentPiece objects in the memory
		// ʹ��"placement new" (�μ�����M8)
		for (int i = 0; i < 10; ++i)
					new (&bestPieces[i]) EquipmentPiece( ID Number );
					
	  //ʹ��placement new��ȱ������Ǵ��������Ա��������Ϥ��
	  //����ʹ�����͸����ˣ������о��ǵ��㲻��������������ʹ��ʱ��
	  //�����ֶ�����������������������Ȼ����ò�����delete[]���ͷ�raw memory
	  
	  // ���빹��bestPieces�����෴��˳��⹹����
			for (int i = 9; i >= 0; --i)
				bestPieces[i].~EquipmentPiece();
		// deallocate the raw memory
			operator delete[](rawMemory);
		//��������������Ҫ���ʹ������ͨ������ɾ��������
		//��ô���������н��ǲ���Ԥ��ġ�������Ϊ��ֱ��ɾ��
		//һ��������new��������������ڴ�ָ�룬����û�б����塣
		delete [] bestPieces; // û�ж���! bestPieces������new����������ġ�
		
		
		//��������û�ж���ȱʡ���캯������ɵĵڶ��������������޷�
		//��������ģ�壨template-based������������ʹ�á���Ϊʵ��
		//��һ��ģ��ʱ��ģ������Ͳ���Ӧ���ṩһ��ȱʡ���캯��������
		//һ��������Ҫ�����Ҫ������������ģ���ڲ�����������ģ���
		//���������������һ������ģ���ࣺ
		
		template<class T>
		class Array {
		public:
			Array(int size);
			...
		private:
			T *data;
		};
		template<class T>
		Array<T>::Array(int size)
		{
			data = new T[size]; // Ϊÿ������Ԫ��
			... //���ε��� T::T()
		}
		//���ṩȱʡ���캯��������࣬����������к�����
		//��Ϊ�������е���������ʵ����ʱ������������
		//���캯���ṩ���������Ҫ��������û��ȱʡ����
		//�����������̳�������������(�����ж�Զ)����
		//��֪��������ṩ������๹�캯���Ĳ����ĺ��塣
		//������������ǲ������κ�ϲ�����ֹ涨�ġ�
		
		
		//�������޸�ʹ��������Ա������ø��ӣ���Ϊ������
		//ȷ��EquipmentPiece���������������ĳ�ʼ������
		//��������һ����û��ID��û�������EquipmentPiece����
		//��ô�������Ա����������ID�Ƿ���ڡ����������ID��
		//���ǽ�����ָ����ô���Ĵ��󡣲���ͨ������ȷӦ����ôȥ����
		//�ܶ�����ʵ��ʲôҲû���ṩ��ֻ���׳�һ���쳣�����һ��
		//������ֹ���򡣵��������η���ʱ������˵�ṩȱʡ���캯��
		//��������һ�ֱ�֤���Ƶ������Ƿ���������������������
		class EquipmentPiece {
			public:
				EquipmentPiece( int IDNumber = UNSPECIFIED);
				...
			private:
				static const int UNSPECIFIED; // ��ֵ����IDֵ��ȷ����
			};
			//��������������EquipmentPiece����
			EquipmentPiece e; //�����Ϸ�