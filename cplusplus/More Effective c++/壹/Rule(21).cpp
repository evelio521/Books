����:21
	/*�����ؼ���������ʽ����ת��*/
	
	//�ñ����������������ת����ȷʵ�Ǻܷ��㣬���ǽ�����ʱ�����������ת��
	//�������п����ģ������ǲ���е����ֿ���������������ֻ�������������
	//�������Ϊ�˸���һ���������C++����Աϣ������û����ʱ����������ʽ
	//����ת�������ǵ�Ŀ�Ĳ������Ҫ��������ת����������UPint��int��Ϊ����
	//����operator+����ʽ����ת��ֻ�������ﵽĿ�ĵ��ֶΣ��������ǲ�Ҫ����
	//�ֶ���Ŀ��
	
	
	//���µĴ��벻�Ǻܺ� ��������ʽת��
	class UPInt { // unlimited precision
		public: // integers ��
			UPInt();
			UPInt(int value);
			...
	};
		//�й�Ϊʲô����ֵ��const�Ľ��ͣ��μ�Effective C++ ����21
	const UPInt operator+(const UPInt& lhs, const UPInt& rhs);
	UPInt upi1, upi2;
	...
	UPInt upi3 = upi1 + upi2;
		
	//���ڿ���������Щ��䣺
	upi3 = upi1 + 10;//ok
	upi3 = 10 + upi2;//ok
	
	//�Ľ����㷨
	const UPInt operator+(const UPInt& lhs, // add UPInt
	const UPInt& rhs); // and UPInt
	const UPInt operator+(const UPInt& lhs, // add UPInt
	int rhs); // and int
	const UPInt operator+(int lhs, // add int and
	const UPInt& rhs); // UPInt
	UPInt upi1, upi2;
	...
	UPInt upi3 = upi1 + upi2; // ��ȷ,û����upi1 �� upi2
	// ���ɵ���ʱ����
	upi3 = upi1 + 10; // ��ȷ, û����upi1 or 10
	// ���ɵ���ʱ����
	upi3 = 10 + upi2; //��ȷ, û����10 or upi2
	//���ɵ���ʱ����
		