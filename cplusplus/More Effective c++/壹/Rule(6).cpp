����6:
	/*M6������(increment)���Լ�(decrement)������ǰ׺��ʽ���׺��ʽ������*/
	class UPInt { // "unlimited precision int"
		public:
			UPInt& operator++(); // ++ ǰ׺
			const UPInt operator++(int); // ++ ��׺
			UPInt& operator--(); // -- ǰ׺
			const UPInt operator--(int); // -- ��׺
			UPInt& operator+=(int); // += ��������UPInts
			// ��ints ������
			...
		};
			UPInt i;
			++i; // ���� i.operator++();
			i++; // ���� i.operator++(0);
			--i; // ���� i.operator--();
			i--; // ���� i.operator--(0);
			
			// ǰ׺��ʽ������Ȼ��ȡ��ֵ
			UPInt& UPInt::operator++()
			{
				*this += 1; // ����
				return *this; // ȡ��ֵ
			}
			// postfix form: fetch and increment
			const UPInt UPInt::operator++(int)
			{
				UPInt oldValue = *this; // ȡ��ֵ
				++(*this); // ����
				return oldValue; // ���ر�ȡ�ص�ֵ
			}
			
			/*
			 * ���ۣ������Ϊ����ߴ���Ч�ʣ�UPInt�ĵ�����Ӧ�þ���ʹ��ǰ׺increment��
			 * ���ú�׺increment������ȷʵ��Ҫʹ�ú�׺increment����������ȷһ�£���
			 * �����û����������ʱ�������ܵ�ʹ��ǰ׺increment����Ϊ����Ч�ʽϸߡ�
			 */