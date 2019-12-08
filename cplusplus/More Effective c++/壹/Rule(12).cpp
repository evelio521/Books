规则:12
	/*了解抛出一个exception与传递一个参数或调用一个虚函数之间的差异*/
	
	/*
	 *你传递函数参数与异常的途径可以是传值、传递引用或传递指针，这是相同
	 *的。但是当你传递参数和异常时，系统所要完成的操作过程则是完全不同的。
	 *产生这个差异的原因是：你调用函数时，程序的控制权最终还会返回到函数
	 *的调用处，但是当你抛出一个异常时，控制权永远不会回到抛出异常的地方。
	 */
	 
	 // 一个函数，从流中读值到Widget中
		istream operator>>(istream& s, Widget& w);
		void passAndThrowWidget()
		{
				Widget localWidget;
				cin >> localWidget; //传递localWidget到 operator>>
				throw localWidget; // 抛出localWidget异常
		}
   /*
    *当传递localWidget到函数operator>>里，不用进行拷贝操作，而是把
    *operator>>内的引用类型变量w指向localWidget，任何对w的操作实际上都
    *施加到localWidget上。这与抛出localWidget异常有很大不同。不论通过
    *传值捕获异常还是通过引用捕获（不能通过指针捕获这个异常，因为类型不
    *匹配）都将进行lcalWidget的拷贝操作，也就说传递到catch子句中的是
    *localWidget的拷贝。必须这么做，因为当localWidget离开了生存空间后，
    *其析构函数将被调用。如果把localWidget本身（而不是它的拷贝）传递给
    *catch子句，这个子句接收到的只是一个被析构了的Widget，一个Widget的“尸体”。
    *这是无法使用的。因此C++规范要求被做为异常抛出的对象必须被复制。
    */
    
    //即使被抛出的对象不会被释放，也会进行拷贝操作。
    //例如如果passAndThrowWidget函数声明localWidget为静态变量（static），
			void passAndThrowWidget()
			{
					static Widget localWidget; // 现在是静态变量（static）;
					//一直存在至程序结束
					cin >> localWidget; // 象以前那样运行
					throw localWidget; // 仍将对localWidget
			} //进行拷贝操作
     /*
      *当抛出异常时仍将复制出localWidget的一个拷贝。这表示即使通过引用来捕获异常，
      *也不能在catch块中修改localWidget；仅仅能修改localWidget的拷贝。对异常对象
      *进行强制复制拷贝，这个限制有助于我们理解参数传递与抛出异常的第二个差异：
      *抛出异常运行速度比参数传递要慢。
      */
      
      //当异常对象被拷贝时，拷贝操作是由对象的拷贝构造函数完成的。
      //该拷贝构造函数是对象的静态类型（static type）所对应类的拷
      //贝构造函数，而不是对象的动态类型（dynamic type）对应类的
      //拷贝构造函数。比如以下这经过少许修改的passAndThrowWidget：
			class Widget { ... };
			class SpecialWidget: public Widget { ... };
			void passAndThrowWidget()
			{
					SpecialWidget localSpecialWidget;
					...
					Widget& rw = localSpecialWidget; // rw 引用SpecialWidget
					throw rw; //它抛出一个类型为Widget
					// 的异常
			}
     /*
      *这里抛出的异常对象是Widget，即使rw引用的是一个SpecialWidget。
      *因为rw的静态类型（static type）是Widget，而不是SpecialWidget。
      *你的编译器根本没有注意到rw引用的是一个SpecialWidget。编译器所
      *注意的是rw的静态类型（static type）。这种行为可能与你所期待的不一样，
      *但是这与在其他情况下C++中拷贝构造函数的行为是一致的。
      */
    
    
    
     catch (Widget& w) // 捕获Widget异常
		{
				... // 处理异常
				throw; // 重新抛出异常，让它
		} // 继续传递
		catch (Widget& w) // 捕获Widget异常
		{
				... // 处理异常
				throw w; // 传递被捕获异常的
		} // 拷贝
		/*
		 *这两个catch块的差别在于第一个catch块中重新抛出的是当前捕获的异常，
		 *而第二个catch块中重新抛出的是当前捕获异常的一个新的拷贝。如果忽略
		 *生成额外拷贝的系统开销，这两种方法还有差异么？
		 *当然有。第一个块中重新抛出的是当前异常（current exception）,无论它是什么类型。
		 *特别是如果这个异常开始就是做为SpecialWidget类型抛出的，那么第一个块中传递出去
		 *的还是SpecialWidget异常，即使w的静态类型（static type）是Widget。这是因为重新
		 *抛出异常时没有进行拷贝操作。第二个catch块重新抛出的是新异常，类型总是Widget，
		 *因为w的静态类型（static type）是Widget。一般来说，你应该用throw来重新抛出当前
		 *的异常，因为这样不会改变被传递出去的异常类型，而且更有效率，因为不用生成一个新拷贝。
		 */
		 
		 //passAndThrowWidgetp抛出的：
			catch (Widget w) ... // 通过传值捕获异常
			catch (Widget& w) ... // 通过传递引用捕获
			// 异常
			catch (const Widget& w) ... //通过传递指向const的引用
																	//捕获异常
			//我们立刻注意到了传递参数与传递异常的另一个差异。一个被异常抛出的对象（刚才解释过，总是一个临时对象）
			//可以通过普通的引用捕获；它不需要通过指向const对象的引用（reference-to-const）捕获。在函数调用中不允
			//许转递一个临时对象到一个非const引用类型的参数里（参见条款M19），但是在异常中却被允许。
			//让我们先不管这个差异，回到异常对象拷贝的测试上来。我们知道当用传值的方式传递函数的参数，我们制造了被
			//传递对象的一个拷贝（参见Effective C++ 条款22），并把这个拷贝存储到函数的参数里。同样我们通过传值的方
			//式传递一个异常时，也是这么做的。当我们这样声明一个catch子句时：
			catch (Widget w) ... // 通过传值捕获
			//会建立两个被抛出对象的拷贝，一个是所有异常都必须建立的临时对象，第二个是把临时对象拷贝进w中（WQ加注，
			//重要：是两个！）。同样，当我们通过引用捕获异常时，
			catch (Widget& w) ... // 通过引用捕获
			catch (const Widget& w) ... //也通过引用捕获
			//这仍旧会建立一个被抛出对象的拷贝：拷贝同样是一个临时对象。相反当我们通过引用传递函数参数时，没有进行
			//对象拷贝。当抛出一个异常时，系统构造的（以后会析构掉）被抛出对象的拷贝数比以相同对象做为参数传递给函
			//数时构造的拷贝数要多一个。
			
			void f(int value)
			{
			try {
			if (someFunction()) { // 如果 someFunction()返回
			throw value; //真，抛出一个整形值
			...
			}
			}
			catch (double d) { // 只处理double类型的异常
			...
			}
			...
			}
		/*
		 *在try块中抛出的int异常不会被处理double异常的catch子句捕获。该子句只能捕获类型真真正正为double的异常，
		 *不进行类型转换。因此如果要想捕获int异常，必须使用带有int或int&参数的catch子句。
		 */
		 
		 //不过在catch子句中进行异常匹配时可以进行两种类型转换。
		   1.//第一种是继承类与基类间的转换。一个用来捕获基类的catch子句也可以处理派生类类型的异常。
		   2.//第二种是允许从一个类型化指针（typed pointer）转变成无类型指针（untyped pointer），
		   //所以带有const void* 指针的catch子句能捕获任何类型的指针类型异常：
				catch (const void*) ... //捕获任何指针类型异常
				
				
			
			
			//传递参数和传递异常间最后一点差别是catch子句匹配顺序总是取决于它们在程序中出现的顺序。
			
			/*
			 *综上所述，把一个对象传递给函数或一个对象调用虚拟函数与把一个对象做为异常抛出，这之间有三个主要区别。
			 *第一、异常对象在传递时总被进行拷贝；当通过传值方式捕获时，异常对象被拷贝了两次。对象做为参数传递给
			 *函数时不一定需要被拷贝。第二、对象做为异常被抛出与做为参数传递给函数相比，前者类型转换比后者要少
			 *（前者只有两种转换形式）。最后一点，catch子句进行异常类型匹配的顺序是它们在源代码中出现的顺序，
			 *第一个类型匹配成功的catch将被用来执行。当一个对象调用一个虚拟函数时，被选择的函数位于与对象类型匹配
			 *最佳的类里，即使该类不是在源代码的最前头。
			 */