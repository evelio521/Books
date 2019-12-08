����:22
	/*�����ò������ĸ�����ʽ��op=��ȡ���䵥����ʽ��op��*/
	
	class Rational {
		public:
			...
			Rational& operator+=(const Rational& rhs);
			Rational& operator-=(const Rational& rhs);
		};
	// operator+ ����operator+=ʵ��;
	//�й�Ϊʲô����ֵ��const�Ľ��ͣ�
	//�μ�Effective C++����21 �� 109ҳ ���й�ʵ�ֵľ���
		const Rational operator+(const Rational& lhs,const Rational& rhs)
		{
			return Rational(lhs) += rhs;
		}
		// operator- ���� operator -= ��ʵ��
		const Rational operator-(const Rational& lhs,const Rational& rhs)
		{
			return Rational(lhs) -= rhs;
		}

		//����㲻��������е�operator�ĵ�����ʽ����ȫ�����
		//�ǾͿ���ʹ��ģ�������������ʽ�ĺ����ı�д��
		template<class T>
		const T operator+(const T& lhs, const T& rhs)
		{
				return T(lhs) += rhs; // �μ����������
		}
		template<class T>
		const T operator-(const T& lhs, const T& rhs)
		{
				return T(lhs) -= rhs; // �μ����������
		}


		//Ч�ʷ�������⡣
		//��һ���ܵ���˵operator�ĸ�ֵ��ʽ���䵥����ʽЧ�ʸ��ߣ�
				//��Ϊ������ʽҪ����һ���¶��󣬴Ӷ�����ʱ����Ĺ����
				//�ͷ�����һЩ�������μ�����M19������M20������Effective 
				//C++����23����operator�ĸ�ֵ��ʽ�ѽ��д����ߵĲ����
				//��˲���Ҫ������ʱ����������operator�ķ���ֵ��
		//�ڶ����ṩoperator�ĸ�ֵ��ʽ��ͬʱҲҪ�ṩ���׼��ʽ��
				//������Ŀͻ����ڱ���
				//��Ч������������ѡ��Ҳ����˵���ͻ��˿��Ծ�����������д��
			Rational a, b, c, d, result;
			...
			result = a + b + c + d; // ��������3����ʱ����
			// ÿ��operator+ ����ʹ��1��
			����������д��
			result = a; //������ʱ����
			result += b; //������ʱ����
			result += c; //������ʱ����
			result += d; //������ʱ����
			
			
			template<class T>
			const T operator+(const T& lhs, const T& rhs)
			{
					T result(lhs); // ����lhs �� result��
					return result += rhs; // rhs������Ӳ����ؽ��
			}
		//���ģ�弸����ǰ��ĳ�����ͬ����������֮�仹�Ǵ�����Ҫ
		//�Ĳ�𡣵ڶ���ģ�����һ����������result�����������
		//����ζ�Ų�����operator+ ��ʹ�÷���ֵ�Ż����μ�����M20����
		//��һ��ʵ�ַ����ܿ���ʹ�÷���ֵ�Ż������Ա�����Ϊ��������
		//������Ŀ��ܾͻ����