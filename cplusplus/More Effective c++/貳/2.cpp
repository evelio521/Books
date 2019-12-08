	#include<iostream>

	class Widget {
	public:
		Widget()
		{
		}
		virtual int temp()
		{
			return 1;
		}
		
	 };
	class SpecialWidget: public Widget { 
	public:
		 int temp()
		{
			return 2;
		}
		
	 };
	void update(SpecialWidget *psw)
	{
	}
	void updateViaRef(SpecialWidget& rsw)
	{
	}
	 int doSomething()
	 {
		 return 3;
	 }
int main()
{
	//static_cast基本拥有旧式c转换的基本功能和意义
	//但是不能去掉表达式的常量行
	int firstNumber=5, secondNumber=6;
	double result = static_cast<double>(firstNumber)/secondNumber;
	
	//const_cast用于类型转换掉表达式的const或volatileness属性
	
		 SpecialWidget sw; // sw 是一个非const 对象。
		 const SpecialWidget& csw = sw; // csw 是sw的一个引用
		 																// 它是一个const 对象
		 
		 	//update(&csw);
		  // 错误!不能传递一个const SpecialWidget* 变量
		 	// 给一个处理SpecialWidget*类型变量的函数
		 	
		 	
		 	
		 update(const_cast<SpecialWidget*>(&csw));
					// 正确，csw的const被显示地转换掉（
					// csw和sw两个变量值在update
				//函数中能被更新）
		 update((SpecialWidget*)&csw);
				// 同上，但用了一个更难识别
				//的C风格的类型转换
		/*
		 Widget *pw = new SpecialWidget;
		 update(pw); // 错误！pw的类型是Widget*，但是
		 							// update函数处理的是SpecialWidget*类型
		 update(const_cast<SpecialWidget*>(pw));
									// 错误！const_cast仅能被用在影响
                  // constness or volatileness的地方上。,
                  // 不能用在向继承子类进行类型转换。
    */
    
    

	    Widget *pw;
		
		update(dynamic_cast<SpecialWidget*>(pw));
					// 正确，传递给update函数一个指针
					// 是指向变量类型为SpecialWidget的pw的指针
					// 如果pw确实指向一个对象,
					// 否则传递过去的将使空指针。
		updateViaRef(dynamic_cast<SpecialWidget&>(*pw));
					//正确。 传递给updateViaRef函数
					// SpecialWidget pw 指针，如果pw
					// 确实指向了某个对象
					// 否则将抛出异常
			//dynamic_casts在帮助你浏览继承层次上是有限制的。它不能被用于缺乏虚函数的类型上（参见条款M24），
			//也不能用它来转换掉constness：
	
	
	//reinterpret_casts的最普通的用途就是在函数指针类型之间进行转换
	typedef void (*FuncPtr)(); // FuncPtr is 一个指向函数
																	// 的指针，该函数没有参数
																	// 返回值类型为void
	FuncPtr funcPtrArray[10]; // funcPtrArray 是一个能容纳
																// 10个FuncPtrs指针的数组
		 
		  
		  //funcPtrArray[0] = &doSomething; // 错误！类型不匹配

      //reinterpret_cast可以让你迫使编译器以你的方法去看待它们：
      funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);// this compiles
	return 0;
}