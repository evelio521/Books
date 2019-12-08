����5:
	/*�Զ��Ƶ�����ת���������־���*/
	
	/*
	 * �����ֺ������������������Щ��ת����
	 * ���������캯����single-argument constructors��
	 * ����ʽ����ת������������������캯����ָֻ��һ��
	 * ���������Ե��õĹ��캯�����ú���������ֻ������һ��
	 * ������Ҳ�������䶨���˶����������һ�������Ժ�����в�������ȱʡֵ��
	 */
	 
	 class Name { // for names of things
		public:
			Name(const string& s); // ת�� string ��
														// Name
			...
		};
	class Rational { // ��������
		public:
			Rational(int numerator = 0, // ת��int��
			int denominator = 1); // ��������
			...
	};
	
	/*
	 * ��ʽ����ת�������ֻ��һ��������ֵĳ�Ա������
	 * operator �ؼ��֣�����һ�����ͷ��š��㲻�ö�
	 * �庯���ķ������ͣ���Ϊ�������;���������������֡�
	 */
	 
	 class Rational {
		public:
			...
			operator double() const; // ת��Rational���
		}; // double����
	//��������������£���������ᱻ�Զ����ã�
	Rational r(1, 2); // r ��ֵ��1/2
	double d = 0.5 * r; // ת�� r ��double,
											// Ȼ�����˷�
									
	/*
	 * �ټ���������ΪRational������operator<<��
	 * ��������ӡ������ʧ�ܣ���Ϊû�к��ʵĵ�operator<<�����á�
	 * ��������ˡ�������������operator<<ʱ���ᷢ��û�������ĺ������ڣ�
	 * ����������ͼ�ҵ�һ�����ʵ���ʽ����ת��˳����ʹ�ú��������������С�
	 * ����ת��˳��Ĺ������Ǹ��ӵģ�������������������£��������ᷢ
	 * �������ܵ���Rational::operator double��������rת��Ϊdouble���͡�
	 * �������������ӡ�Ľ����һ����������������һ�������������ֱ��һ
	 * �����ѣ���������������ʽ����ת����ȱ�㣺���ǵĴ��ڽ����´���ķ�����
	 */		
	Rational r(1, 2);
	cout << r; // Ӧ�ô�ӡ��"1/2"
 
  //����������ò�ʹ���﷨�ؼ��ֵĵ�ͬ�ĺ��������ת�������
  class Rational {
		public:
			...
			double asDouble() const; //ת�� Rational
		}; // ��double
	//�����Ա�����ܱ���ʽ���ã�
		Rational r(1, 2);
		cout << r; // ����! Rationa����û��
							// operator<<
		cout << r.asDouble(); // ��ȷ, ��double����
													//��ӡr
													
		/*
		 * ��һ�����캯�����������ȷ�����������ķ�Χ�������10��20��
		 * ����һ�����������캯�������Բ�����Ϊ����ת���������ڶ���
		 * ���캯���õ����߽�����������Ԫ�صĸ�����ʹ�÷�������������
		 * ��ʹ�����ƣ���������ͬ����������Ϊ����ת������ʹ�ã��ܵ��������ʹ�ࡣ
		 */
		template<class T>
			class Array {
				public:
					Array(int lowBound, int highBound);
					Array(int size);
					T& operator[](int index);
					...
			};
		//�������´���
		bool operator==( const Array<int>& lhs,
		const Array<int>& rhs);
		Array<int> a(10);
		Array<int> b(10);
		...
		for (int i = 0; i < 10; ++i)
		if (a == b[i]) { // ����! "a" Ӧ���� "a[i]"
			do something for when
			a[i] and b[i] are equal;
		}
		else {
			do something for when they are not;
}
		//��ִ��һ�´���
		for (int i = 0; i < 10; ++i)
    if (a == static_cast< Array<int> >(b[i])) ...
    	
    //����취 ʹ�ùؼ���explicit
    template<class T>
		class Array {
			public:
				...
				explicit Array(int size); // ע��ʹ��"explicit"
				...
		};
		Array<int> a(10); // ��ȷ, explicit ���캯��
											// �ڽ�������ʱ������ʹ��
		Array<int> b(10); // Ҳ��ȷ
		if (a == b[i]) ... // ����! û�а취
											// ��ʽת��
											// int �� Array<int>
		if (a == Array<int>(b[i])) ... // ��ȷ,��ʽ��int��
																	// Array<int>ת��
																	// �����Ǵ�����߼�
																// ������
		if (a == static_cast< Array<int> >(b[i])) ...
									// ͬ����ȷ��ͬ��
										// ������
		if (a == (Array<int>)b[i]) ... //C����ת��Ҳ��ȷ��
															// �����߼�
													// ���ɲ�����
													
		//����Ľ���취
		template<class T>
		class Array {
			public:
				class ArraySize { // ��������µ�
					public:
						ArraySize(int numElements): theSize(numElements) {}
						int size() const { return theSize; }
					private:
						int theSize;
					};
				Array(int lowBound, int highBound);
				Array(ArraySize size); // ע���µ�����
				...
		};
    /*
     * ��ı�����Ҫ����int��������Array<int>��Ĺ��캯��������û�������Ĺ��캯����
     * ��������ʶ�����ܴ�int����ת����һ����ʱArraySize����ArraySize����ֻ��
     * Array<int>���캯������Ҫ�ģ�����������������ת�����������ã������Ķ�������Ҳ�ͳɹ��ˡ�
     */
     
     bool operator==( const Array<int>& lhs,const Array<int>& rhs);
			Array<int> a(10);
			Array<int> b(10);
			...
		for (int i = 0; i < 10; ++i)
				if (a == b[i]) ... // ����! "a" Ӧ���� "a[i]";
														// ������һ������