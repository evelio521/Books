/*�Զ��������Դ*/
 class Investment{
 	
};
Investment *createInvestment();//��������

//���ܺ�����...���������쳣���޷�ɾ��pInv
void f()
{
	Investment *pInv=createInvestment();
	...
	delete pInv;
}
//����auto_ptr�������������

void f()
{
	std::auto_ptr<Investment> pInv(createInvestment());
	...
	
}

//auto_ptr �� shared_ptr������

  std::auto_ptr<Investment> pInv1(createInvestment());//pInv1ָ��createInvestment()�Ķ���
  std::auto_ptr<Investment> pInv2(pInv1);//pInv2ָ�����pInv1����Ϊnull
  pInv1=pInv2;//pInv1ָ�����pInv2����Ϊnull
  	
  std::tr1::shared_ptr<Investment> pInv1(createInvestment());//pInv1ָ��createInvestment()�Ķ���
  std::tr1::shared_ptr<Investment> pInv2(pInv1);//pInv2ָ�����pInv1ָ�����
  pInv1=pInv2;//pInv2ָ�����pInv1ָ�����
  
  