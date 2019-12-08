/*以对象管理资源*/
 class Investment{
 	
};
Investment *createInvestment();//工厂函数

//可能函数在...部分遇到异常而无法删除pInv
void f()
{
	Investment *pInv=createInvestment();
	...
	delete pInv;
}
//利用auto_ptr来避免此类问题

void f()
{
	std::auto_ptr<Investment> pInv(createInvestment());
	...
	
}

//auto_ptr 和 shared_ptr的区别

  std::auto_ptr<Investment> pInv1(createInvestment());//pInv1指向createInvestment()的对象
  std::auto_ptr<Investment> pInv2(pInv1);//pInv2指向对象，pInv1设置为null
  pInv1=pInv2;//pInv1指向对象，pInv2设置为null
  	
  std::tr1::shared_ptr<Investment> pInv1(createInvestment());//pInv1指向createInvestment()的对象
  std::tr1::shared_ptr<Investment> pInv2(pInv1);//pInv2指向对象，pInv1指向对象
  pInv1=pInv2;//pInv2指向对象，pInv1指向对象
  
  