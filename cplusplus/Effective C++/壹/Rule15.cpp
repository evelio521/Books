/*����Դ�����ṩ��ԭʼ��Դ�ķ���*/

std::tr1::shared_ptr<Investment> pInv(createInvestment());
int daysHeld(const Investment *pi);//����Ͷ������
int days=daysHeld(pInv);//error

//auto_ptr��tr1::shared_ptr�ṩget��������������ָ���ڲ���ԭʼָ��
int days=daysHeld(pInv.get());

//auto_ptr��tr1::shared_ptr������ operator->��operatot*

class Investment{
	
	public:
		bool isFree()const;
		...
}

Investment *createInvestment();

std::tr1::shared_ptr<Investment> pInv1(createInvestment());
	bool tab1=!(pInv1->isFree());
std::auto_ptr<Investment> pInv1(createInvestment());
	bool tab2=!( (*pInv2).isFree());
