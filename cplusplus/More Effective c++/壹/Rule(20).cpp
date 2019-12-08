����:20
  /*Э����ɷ���ֵ�Ż�*/
  
  class Rational {
		public:
			Rational(int numerator = 0, int denominator = 1);
			...
			int numerator() const;
			int denominator() const;
	};
		// �й�Ϊʲô����ֵ��const�Ľ��ͣ��μ�����M6,
		const Rational operator*(const Rational& lhs,const Rational& rhs);
		
		// һ�ֲ�����ı��ⷵ�ض���ķ���
		const Rational * operator*(const Rational& lhs,
		const Rational& rhs);
		Rational a = 10;
		Rational b(1, 2);
		Rational c = *(a * b); //����������ܡ�������ô��
		
		//һ��Σ�յ�(�Ͳ���ȷ��)�������������ⷵ�ض���
		const Rational& operator*(const Rational& lhs,
		const Rational& rhs);
		Rational a = 10;
		Rational b(1, 2);
		Rational c = a * b; // ����ȥ�ܺ���
				
		// ��һ��Σ�յķ��� (�Ͳ���ȷ��)����������
		// ���ⷵ�ض���
		const Rational& operator*(const Rational& lhs,const Rational& rhs)
		{
		Rational result(lhs.numerator() * rhs.numerator(),
		lhs.denominator() * rhs.denominator());
		return result;//WQ��ע ����ʱ����ָ��Ķ����Ѿ���������
		}
		
		// һ�ָ�Ч����ȷ�ķ���������ʵ��
		// ���ض���ĺ���
		const Rational operator*(const Rational& lhs,
		const Rational& rhs)
		{
				return Rational(lhs.numerator() * rhs.numerator(),
				lhs.denominator() * rhs.denominator());
		}
		
		Rational a = 10;
		Rational b(1, 2);
		Rational c = a * b; // ���������operator*
		//�������ͻᱻ����������operator*�ڵ���ʱ������operator*��
		//�ص���ʱ��������������ΪĿ��c������ڴ��ﹹ��return���ʽ
		//����Ķ��������ı���������ȥ��������operator*����ʱ��
		//��Ŀ��������㣺û�н�����ʱ������Ĵ��۾��ǵ���һ������
		//�����D�D����cʱ���õĹ��캯��
		
		// the most efficient way to write a function returning
		// an object
			inline const Rational operator*(const Rational& lhs,
			const Rational& rhs)
			{
					return Rational(lhs.numerator() * rhs.numerator(),
					lhs.denominator() * rhs.denominator());
			}