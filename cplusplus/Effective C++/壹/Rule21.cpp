/*���뷵�ض���ʱ�������뷵��reference*/

class Rational{
	public:
		Rational(int numerator=0,int denominator=1);
		...
	private:
		int n,d;
		friend const Rational operator*(const Rational& lhs,const Rational& rhs);
};

//�������reference

const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	Rational result(lhs.n*rhs.n,lhs.d*rhs.d);//���Ĵ���
	return result;
}

const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	Rational *result=new Rational(lhs.n*rhs.n,lhs.d*rhs.d);//�����Ĵ���
	return *result;
}

const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	static Reational result;//����һ�������Ĵ���
	
	result=...;

	return result;
}
//��������Ĵ���
bool operator==(const Rational& lhs,const Rational& rhs);

Rational a,b,c,d;

if((a*b)==(c*d))//����Զ������ֵ��������ͼ��̲�
	{
	}
else
	{
	}
	
