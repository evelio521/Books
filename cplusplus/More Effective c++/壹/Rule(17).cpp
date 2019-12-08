����:17
	/*����ʹ�û�ʽ����*/
	
	//1.���ü���
	
		class String { ... }; // һ��string �� (the standard
		// string type may be implemented
		// as described below, but it
		// doesn't have to be)
		String s1 = "Hello";
		String s2 = s1; / ����string�������캯��
		//ͨ��string�������캯����s2��s1��ʼ����s1��s2�����Լ��ġ�Hello��������
		//���ֿ������캯��������ϴ�Ŀ�������ΪҪ����s1ֵ�Ŀ���������ֵ����s2��
		//��ͨ����Ҫ��new������������ڴ�(�μ�����8)����Ҫ����strcpy��������s1�ڵ����ݵ�s2��
		
		//����һ��eager evaluation��������㣩��ֻ��Ϊ��string�������캯������
		//Ҫ����s1ֵ�Ŀ�������������s2��Ȼ����ʱ��s2������Ҫ���ֵ�Ŀ�������Ϊs2û�б�ʹ�á�
		//�����ܾ����ٹ�������Ӧ�ø���s2һ��s1�Ŀ�����������s2��s1����һ��ֵ��
		//����ֻ����һЩ��¼�Ա�֪��˭�ڹ���ʲô�����ܹ�ʡ������new�Ϳ����ַ��Ŀ�������ʵ��
		//s1��s2����һ�����ݽṹ�������client��˵��͸���ģ����������������˵����û��ʲô
		//�����Ϊ����ֻ�Ƕ����ݣ�
			cout << s1; // ��s1��ֵ
			cout << s1 + s2; // ��s1��s2��ֵ
			
	  	s2.convertToUpperCase();
			//�������ؽ�Ҫ�ģ������޸�s2��ֵ����������s1��ֵһ���޸ġ�
			//Ϊ������ִ����䣬string��convertToUpperCase����Ӧ������s2ֵ��һ��������
			//���޸�ǰ�����˽�е�ֵ����s2����convertToUpperCase�ڲ������ǲ����������ˣ�
			//����Ϊs2������ģ�ֵ������������s2�Լ�ʹ�á���һ���棬������޸�s2��
			//���ǾͲ����������Լ�ֵ�Ŀ������������ֹ���ֱֵ�������˳���������Ǻ����ˣ�
			//s2���ᱻ�޸ģ����������������ԶҲ����Ϊ������������ֵ�ķѾ�����

       //���ֹ���ֵ������ʵ��ϸ�ڣ��������еĴ��룩������M29�б��ṩ���������̺���ԭ�����lazy
			//evaluation��������ȷʵ��Ҫ����ȥΪ�κζ�����������������Ӧ��������ģ�
			//ֻҪ���ܾ͹���ʹ������ֵ����һЩӦ�������㾭��������ô����
			
	//2.����Դ���ȡ��д��
				String s = "Homer's Iliad"; // ������һ��
			// reference-counted string
			...
			cout << s[3]; // ���� operator[] ��ȡs[3]
			s[3] = 'x'; // ���� operator[] д�� s[3]
		//���ȵ���operator[]������ȡstring�Ĳ���ֵ�����ǵڶ��ε��øú�����Ϊ�����д������
		//����Ӧ�ܹ�����Դ������ú�д���ã���Ϊ��ȡreference-counted string�Ǻ����׵ģ�
		//��д�����string����Ҫ��д��ǰ�Ը�stringֵ����һ���¿���������30�����Ž���


 //3.Lazy Fetching��������ȡ��
 		class LargeObject { // ���ͳ־ö���
				public:
						LargeObject(ObjectID id); // �Ӵ����лָ�����
						const string& field1() const; // field 1��ֵ
						int field2() const; // field 2��ֵ
						double field3() const; // ...
						const string& field4() const;
						const string& field5() const;
						...
			};
		//���ڿ���һ�´Ӵ����лָ�LargeObject�Ŀ�����
		void restoreAndProcessObject(ObjectID id)
		{
				LargeObject object(id); // �ָ�����
				...
		}
		//��ΪLargeObject����ʵ���ܴ�Ϊ�����Ķ����ȡ���е����ݣ�
		//���ݿ�Ĳ����Ŀ������ǳ����ر��������Զ�����ݿ��л�ȡ
		//���ݺ�ͨ�����緢������ʱ��������������£�����Ҫ��ȥ�������ݡ�
		//���磬��������һ������
		void restoreAndProcessObject(ObjectID id)
		{
				LargeObject object(id);
				if (object.field2() == 0) {
						cout << "Object " << id << ": null field2.\n";
				}
		}
		
		//���������Ҫfiled2��ֵ������Ϊ��ȡ�����ֶζ�������Ŭ�������˷ѡ�
		
		   //�������
		   class LargeObject {
					public:
							LargeObject(ObjectID id);
							const string& field1() const;
							int field2() const;
							double field3() const;
							const string& field4() const;
							...
					private:
							ObjectID oid;
							mutable string *field1Value; //�μ������й�
							mutable int *field2Value; // "mutable"������
							mutable double *field3Value;
							mutable string *field4Value;
							...
				};
				LargeObject::LargeObject(ObjectID id)
				: oid(id), field1Value(0), field2Value(0), field3Value(0), ...
				{}
				const string& LargeObject::field1() const
				{
						if (field1Value == 0) {
						�����ݿ���Ϊfiled 1��ȡ���ݣ�ʹfield1Value ָ�����ֵ;
					}
					return *field1Value;
				}
				//������ÿ���ֶζ���һ��ָ�����ݵ�ָ������ʾ��LargeObject����
				//������ÿ��ָ���ʼ��Ϊ�ա���Щ��ָ���ʾ�ֶλ�û�д����ݿ���
				//��ȡ��ֵ��ÿ��LargeObject��Ա�����ڷ����ֶ�ָ����ָ�������
				//֮ǰ�����ֶ�ָ�����״̬�����ָ��Ϊ�գ��ڶ����ݽ��в���֮
				//ǰ��������ݿ��ж�ȡ��Ӧ�����ݡ�
				
	//4.Lazy Expression Evaluation(������ʽ����)
	  
	  template<class T>
		class Matrix { ... }; // for homogeneous matrices
		Matrix<int> m1(1000, 1000); // һ�� 1000 �� 1000 �ľ���
		Matrix<int> m2(1000, 1000); // ͬ��
		...
		Matrix<int> m3 = m1 + m2; // m1��m2
		
		//lazy evaluation����˵����������̫�࣬���Ի��ǲ�Ҫȥ����
		//����Ӧ�ý���һ�����ݽṹ����ʾm3��ֵ��m1��m2�ĺͣ�����
		//һ��enum��ʾ���Ǽ��Ǽӷ������������ԣ�����������ݽṹ
		//��m1��m2���Ҫ����࣬Ҳ�ܹ���ʡ�������ڴ档
		
		//���ǳ�������ⲿ�����ݣ���ʹ��m3֮ǰ������ִ�����£�
		Matrix<int> m4(1000, 1000);
		... // ����m4һЩֵ
		m3 = m4 * m1;
		//�������ǿ�������m3��m1��m2�ĺͣ���˽�ʡ�˼���Ŀ�������
		//����������Ӧ�ü�סm3��m4��m1����Ľ��������˵�����ǲ�
		//�ý��г˷����㡣��Ϊ����������ģ����ǵ�ô��
		
		cout << m3[4]; // ��ӡm3�ĵ�����
		//�����ԣ����ǲ����������ˣ�Ӧ�ü���m3�ĵ�����ֵ��������
		//��Ҳ�������Ĺ�������û�����ɼ���m3����������Ľ����
		//m3����Ĳ����Ծɱ���δ�����״ֱ̬��ȷʵ��Ҫ���ǵ�ֵ��
		//�����ˣ�����һֱ����Ҫ��
		
		//�����ؽ���������ʱҲ��ʧ�ܡ��������ʹ��m3��
		cout << m3; // ��ӡm3���е�ֵ
		//һ�ж����ˣ����Ǳ������m3��ȫ����ֵ��ͬ������޸�m3����
		//������һ����������Ҳ�����������㣺
		m3 = m1 + m2; // ��סm3��m1��m2�ĺ�
		m1 = m4; // ����m3��m2��m1�ľ�ֵ֮�ͣ�
		
		//Ϊlazy evaluation������ֵ�õġ����Ի�ȡ������ʱ��Ϳռ�