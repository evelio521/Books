/*在资源类中提供对原始资源的访问*/

std::tr1::shared_ptr<Investment> pInv(createInvestment());
int daysHeld(const Investment *pi);//返回投资天数
int days=daysHeld(pInv);//error

//auto_ptr和tr1::shared_ptr提供get函数来返回智能指针内部的原始指针
int days=daysHeld(pInv.get());

//auto_ptr和tr1::shared_ptr重载了 operator->和operatot*

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
