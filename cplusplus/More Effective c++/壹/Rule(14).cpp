规则:14
	/*明智而审慎的使用exception specification*/
	
	/*
	 *不幸的是，我们很容易就能够编写出导致发生这种灾难的函数。编译器仅仅部分地检测异常
	 *的使用是否与异常规格保持一致。一个函数调用了另一个函数，并且后者可能抛出一个违反
	 *前者异常规格的异常，（A函数调用B函数，但因为B函数可能抛出一个不在A函数异常规格之
	 *内的异常，所以这个函数调用就违反了A函数的异常规格 译者注）编译器不对此种情况进行
	 *检测，并且语言标准也禁止编译器拒绝这种调用方式.
	 */
	 	extern void f1(); // 可以抛出任意的异常
		//假设有一个函数f2通过它的异常规格来声明其只能抛出int类型的异常：
		void f2() throw(int);
		//f2调用f1是非常合法的，即使f1可能抛出一个违反f2异常规格的异常：
		void f2() throw(int)
		{
		...
				f1(); // 即使f1可能抛出不是int类型的
				//异常，这也是合法的。
				...
		}
		
		//使用exception specification的明智选择
		1.避免将exception specification放在需要类型自变量的template身上
		  // 一个不良的template设计 因为他带有exception specification
			template<class T>
			bool operator==(const T& lhs, const T& rhs) throw()
			{
					return &lhs == &rhs;
			}
			
			/* 
			 *这个模板包含的异常规格表示模板生成的函数不能抛出异常。但是事实可能不会这样，因
			 *为opertor&(地址操作符,参见Effective C++ 条款45)能被一些类型对象重载。如果被重
			 *载的话，当调用从operator==函数内部调用opertor&时，opertor&可能会抛出一个异常，
			 *这样就违反了我们的异常规格，使得程序控制跳转到unexpected。上述的例子是一种更一
			 *般问题的特例，这个更一般问题也就是没有办法知道某种模板类型参数抛出什么样的异常。
			 *我们几乎不可能为一个模板提供一个有意义的异常规格。因为模板总是采用不同的方法使
			 *用类型参数。解决方法只能是模板和异常规格不要混合使用。
			 */
		2.如果A函数内调用了B函数，而B函数无exception specification，那么A函数本身也不要设定exception specification。
		  // 一个window系统回调函数指针
			//当一个window系统事件发生时
			typedef void (*CallBackPtr)(int eventXLocation,
			int eventYLocation,
			void *dataToPassBack);
			//window系统类，含有回调函数指针，
			//该回调函数能被window系统客户注册
			class CallBack {
					public:
						CallBack(CallBackPtr fPtr, void *dataToPassBack): func(fPtr), data(dataToPassBack) {}
						void makeCallBack(int eventXLocation,
						int eventYLocation) const throw();
				private:
						CallBackPtr func; // function to call when
						// callback is made
						void *data; // data to pass to callback
			}; // function
			// 为了实现回调函数，我们调用注册函数，
			//事件的作标与注册数据做为函数参数。
			void CallBack::makeCallBack(int eventXLocation,
			int eventYLocation) const throw()
			{
					func(eventXLocation, eventYLocation, data);
			}
			//这里在makeCallBack内调用func，要冒违反异常规格的风险，因为无法知道func会抛出什么类型的异常。
			//通过在程序在CallBackPtr typedef中采用更严格的异常规格来解决问题：
			typedef void (*CallBackPtr)(int eventXLocation,int eventYLocation,void *dataToPassBack) throw();
			
			//这样定义typedef后，如果注册一个可能会抛出异常的callback函数将是非法的：
				// 一个没有异常规格的回调函数
				void callBackFcn1(int eventXLocation, int eventYLocation,
				void *dataToPassBack);
				void *callBackData;
				...
				CallBack c1(callBackFcn1, callBackData);
				//错误！callBackFcn1可能
				// 抛出异常
				//带有异常规格的回调函数
				void callBackFcn2(int eventXLocation,
				int eventYLocation,
				void *dataToPassBack) throw();
				CallBack c2(callBackFcn2, callBackData);
				// 正确，callBackFcn2
				// 没有异常规格
			3.是处理系统本身抛出的exception。这些异常中最常见的是bad_alloc，当内存分配失败时它被
				operator new 和operator new[]抛出（参见条款M8）。如果你在函数里使用new操作符（还参见条款M8），
				你必须为函数可能遇到bad_alloc异常作好准备。
				
		
		
		//解决办法、
		//虽然防止抛出unexpected异常是不现实的，但是C++允许你用其它不同的异常类型替换unexpected异常，
		//你能够利用这个特性。例如你希望所有的unexpected异常都被替换为UnexpectedException对象。你能这样编写代码：
			class UnexpectedException {}; // 所有的unexpected异常对象被
			//替换为这种类型对象
			void convertUnexpected() // 如果一个unexpected异常被
			{ // 抛出，这个函数被调用
					throw UnexpectedException();
			}
			//通过用convertUnexpected函数替换缺省的unexpected函数，来使上述代码开始运行。：
			set_unexpected(convertUnexpected);
			
			//当你这么做了以后，一个unexpected异常将触发调用convertUnexpected函数。Unexpected异常被
			//一种UnexpectedException新异常类型替换。如果被违反的异常规格包含UnexpectedException异常，
			//那么异常传递将继续下去，好像异常规格总是得到满足。（如果异常规格没有包含UnexpectedException，
			//terminate将被调用，就好像你没有替换unexpected一样）
			
			//另一种把unexpected异常转变成知名类型的方法是替换unexpected函数，让其重新抛出当前异常，
			//这样异常将被替换为bad_exception。你可以这样编写：
				void convertUnexpected() // 如果一个unexpected异常被
				{ //抛出，这个函数被调用
				throw; // 它只是重新抛出当前
				} // 异常
				set_unexpected(convertUnexpected);
				// 安装 convertUnexpected
				// 做为unexpected
				// 的替代品
			
			//如果这么做，你应该在所有的异常规格里包含bad_exception（或它的基类，标准类exception）。
			//你将不必再担心如果遇到unexpected异常会导致程序运行终止。任何不听话的异常都将被替换为
			//bad_exception，这个异常代替原来的异常继续传递。
			
			
			class Session { // for modeling online
			public: // sessions
				~Session();
				...
			private:
				static void logDestruction(Session *objAddr) throw();
			};
			Session::~Session()
			{
				try {
					logDestruction(this);
				}
				catch (...) { }
			}
			
			/*
			 *session的析构函数调用logDestruction记录有关session对象被释放的信息，它明确地要捕获从
			 *logDestruction抛出的所有异常。但是logDestruction的异常规格表示其不抛出任何异常。现在
			 *假设被logDestruction调用的函数抛出了一个异常，而logDestruction没有捕获。我们不会期望发
			 *生这样的事情，但正如我们所见，很容易就会写出违反异常规格的代码。当这个异常通过logDestruction
			 *传递出来，unexpected将被调用，缺省情况下将导致程序终止执行。这是一个正确的行为，但这是session
			 *析构函数的作者所希望的行为么？作者想处理所有可能的异常，所以好像不应该不给session析构函数里的
			 *catch块执行的机会就终止程序。如果logDestruction没有异常规格，这种事情就不会发生（一种防止的方
			 *法是如上所描述的那样替换unexpected）。
			 */