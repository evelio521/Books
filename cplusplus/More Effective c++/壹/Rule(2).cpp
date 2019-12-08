规则2:
	/*最好使用c++转型操作符*/
	/*
	 *C++通过引进四个新的类型转换操作符克服了C风格类型转换的缺点，
	 *这四个操作符是, static_cast, const_cast, dynamic_cast, 和reinterpret_cast。
	 *在大多数情况下，对于这些操作符你只需要知道原来你习惯于这样写，
   *         (type) expression
   *而现在你总应该这样写：
   *          static_cast<type>(expression)
	 *
	 */
	 
	 /*
	  *static_cast在功能上基本上与C风格的类型转换一样强大，含义也一样。
	  *它也有功能上限制。例如，你不能用static_cast象用C风格的类型转换
	  *一样把struct转换成int类型或者把double类型转换成指针类型，
	  *另外，static_cast不能从表达式中去除const属性，
	  *因为另一个新的类型转换操作符const_cast有这样的功能。
	  */
	  int firstNumber, secondNumber;
		...
		double result = ((double)firstNumber)/secondNumber；
		//如果用上述新的类型转换方法，你应该这样写：
		double result = static_cast<double>(firstNumber)/secondNumber;
		
		/*
		 *const_cast用于类型转换掉表达式的const或volatileness属性。
		 *通过使用const_cast，你向人们和编译器强调你通过类型转换想做的
		 *只是改变一些东西的constness或者 volatileness属性。这个含义被编译器所约束。
		 *如果你试图使用const_cast来完成修改constness 或者volatileness属性之外的事情，你的类型转换将被拒绝
		 */
		 class Widget { ... };
		 class SpecialWidget: public Widget { ... };
		 void update(SpecialWidget *psw);
		 SpecialWidget sw; // sw 是一个非const 对象。
		 const SpecialWidget& csw = sw; // csw 是sw的一个引用
		 																// 它是一个const 对象
		 update(&csw); // 错误!不能传递一个const SpecialWidget* 变量
		 							// 给一个处理SpecialWidget*类型变量的函数
		 update(const_cast<SpecialWidget*>(&csw));
												// 正确，csw的const被显示地转换掉（
												// csw和sw两个变量值在update
												//函数中能被更新）
		 update((SpecialWidget*)&csw);
												// 同上，但用了一个更难识别
												//的C风格的类型转换
		 Widget *pw = new SpecialWidget;
		 update(pw); // 错误！pw的类型是Widget*，但是
		 							// update函数处理的是SpecialWidget*类型
		 update(const_cast<SpecialWidget*>(pw));
									// 错误！const_cast仅能被用在影响
                  // constness or volatileness的地方上。,
                  // 不能用在向继承子类进行类型转换。
                  
     /*
      *第二种特殊的类型转换符是dynamic_cast，它被用于安全地沿着类的继承关系向下进行类型转换。
      *这就是说，你能用dynamic_cast把指向基类的指针或引用转换成指向其派生类或其兄弟类的指针或引用，
      *而且你能知道转换是否成功。失败的转换将返回空指针（当对指针进行类型转换时）
      *或者抛出异常（当对引用进行类型转换时）：
      */
      Widget *pw;
			...
			update(dynamic_cast<SpecialWidget*>(pw));
					// 正确，传递给update函数一个指针
					// 是指向变量类型为SpecialWidget的pw的指针
					// 如果pw确实指向一个对象,
					// 否则传递过去的将使空指针。
			void updateViaRef(SpecialWidget& rsw);
			updateViaRef(dynamic_cast<SpecialWidget&>(*pw));
					//正确。 传递给updateViaRef函数
					// SpecialWidget pw 指针，如果pw
					// 确实指向了某个对象
					// 否则将抛出异常
			//dynamic_casts在帮助你浏览继承层次上是有限制的。它不能被用于缺乏虚函数的类型上（参见条款M24），
			//也不能用它来转换掉constness：
			int firstNumber, secondNumber;
			...
			double result = dynamic_cast<double>(firstNumber)/secondNumber;
							// 错误！没有继承关系
			const SpecialWidget sw;
			...
			update(dynamic_cast<SpecialWidget*>(&sw));
			       // 错误! dynamic_cast不能转换掉const。
			       
			/*
			 *这四个类型转换符中的最后一个是reinterpret_cast。
			 *使用这个操作符的类型转换，其的转换结果几乎都是执行期定义（implementation-defined）。
			 *因此，使用reinterpret_casts的代码很难移植。
       *reinterpret_casts的最普通的用途就是在函数指针类型之间进行转换
       */
       
      typedef void (*FuncPtr)(); // FuncPtr is 一个指向函数
																	// 的指针，该函数没有参数
																	// 返回值类型为void
			FuncPtr funcPtrArray[10]; // funcPtrArray 是一个能容纳
																// 10个FuncPtrs指针的数组
		  int doSomething();
		  
		  funcPtrArray[0] = &doSomething; // 错误！类型不匹配
      //reinterpret_cast可以让你迫使编译器以你的方法去看待它们：
      funcPtrArray[0] = reinterpret_cast<FuncPtr>(&doSomething);// this compiles
      