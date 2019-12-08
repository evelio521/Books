#include<iostream>
class Rational{
	public:
		
		Rational(int numerator=0,int denominator=1):n(numerator),d(denominator){}

	private:
		int n,d;
		//������const Rational& operator*(const Rational& lhs,const Rational& rhs) �������ѷ���
		//���뷵�ض���ʱ  ��Ҫ���뷵��һ��reference
		friend const Rational& operator*(const Rational& lhs,const Rational& rhs);
		friend bool operator==(const Rational& lhs,const Rational& rhs);
};

//�������reference
/*
//���Բ�Ҫ����pointer��referenceָ��һ��local stack ���󣬻򷵻�referenceָ��һ��heap-allocated����
//�򷵻�pointer��referenceָ��һ��local static����
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
*/
const Rational& operator*(const Rational& lhs,const Rational& rhs)
{
	static Rational result(lhs.n*rhs.n,lhs.d*rhs.d);//����һ�������Ĵ���
	
	
	return result;
}
//��������Ĵ���
bool operator==(const Rational& lhs,const Rational& rhs){

	if(lhs.n==rhs.n&&lhs.d==rhs.d)
		return true;
	else
		return false;
}


	
int main(){
Rational a(3,5),b(2,8),c(4,9),d(8,5);

if((a*b)==(c*d))//����Զ������ֵ��������ͼ��̲�
	{
		std::cout<<"���"<<std::endl;
	}
else
	{
		std::cout<<"�����"<<std::endl;
	}
}