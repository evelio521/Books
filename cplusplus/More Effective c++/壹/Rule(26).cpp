规则26:
	/*限制某个类所能产生的对象数量*/
	//禁止产生对象
	class CantBeInstantiated {
		private:
			CantBeInstantiated();
			CantBeInstantiated(const CantBeInstantiated&);
			...
	};
	
	//只能产生一个对象
	class PrintJob; // forward 声明
		// 参见Effective C++条款34
	class Printer {
		public:
			void submitJob(const PrintJob& job);
			void reset();
			void performSelfTest();
			...
			friend Printer& thePrinter();
		private:
			Printer();
			Printer(const Printer& rhs);
			...
	};
	Printer& thePrinter()
	{
		static Printer p; // 单个打印机对象
		return p;
	}
	
	/*
	 * 这个设计由三个部分组成，第一、Printer类的构造函数是private。
	 * 这样能阻止建立对象。第二、全局函数thePrinter被声明为类的友元，
	 * 让thePrinter避免私有构造函数引起的限制。最后thePrinter包含一个
	 * 静态Printer对象，这意味着只有一个对象被建立。
	 */
	 
	 class PrintJob {
			public:
				PrintJob(const string& whatToPrint);
				...
		};
		string buffer;
				... //填充buffer
		thePrinter().reset();
		thePrinter().submitJob(buffer);
		
		//另一种产生一个对象的方法
		class Printer {
			public:
				static Printer& thePrinter();
				...
			private:
				Printer();
				Printer(const Printer& rhs);
				...
		};
		Printer& Printer::thePrinter()
		{
			static Printer p;
			return p;
		}
		//用户使用printer时有些繁琐：
		Printer::thePrinter().reset();
		Printer::thePrinter().submitJob(buffer);
			
		//加入namespace中
		namespace PrintingStuff {
			class Printer { // 在命名空间
				public: // PrintingStuff中的类
					void submitJob(const PrintJob& job);
					void reset();
					void performSelfTest();
					...
					friend Printer& thePrinter();
			private:
					Printer();
					Printer(const Printer& rhs);
					...
		};
	Printer& thePrinter() // 这个函数也在命名空间里
		{//不能使用inline 后果见教材
			static Printer p;
			return p;
		}
}// 命名空间到此结束

	//使用这个命名空间后，客户端可以通过使用fully-qualified name（完全限制符名）（即包括命名空间的名字），
			PrintingStuff::thePrinter().reset();
			PrintingStuff::thePrinter().submitJob(buffer);
	//但是也可以使用using声明，以简化键盘输入：
			using PrintingStuff::thePrinter; // 从命名空间"PrintingStuff"
															//引入名字"thePrinter"
															// 使其成为当前域
			thePrinter().reset(); // 现在可以象使用局部命名
			thePrinter().submitJob(buffer); // 一样，使用thePrinter
			
			//产生一个对象的另一种写法
			class Printer {
					public:
						class TooManyObjects{}; // 当需要的对象过多时
																		// 就使用这个异常类
						Printer();
						~Printer();
						...
					private:
						static size_t numObjects;
						Printer(const Printer& rhs); // 这里只能有一个printer，
																				// 所以不允许拷贝
			}; // （参见Effective C++ 条款27）
		//此法的核心思想就是使用numObjects跟踪Pritner对象存在的数量。当构造类时，它的值就增加，释放类时，它的值就减少。如果试图构造过多的Printer对象，就会抛出一个TooManyObjects类型的异常：
		// Obligatory definition of the class static
			size_t Printer::numObjects = 0;
			Printer::Printer()
			{
				if (numObjects >= 1) {
				 throw TooManyObjects();
			}
		//继续运行正常的构造函数;
			++numObjects;
			}
			Printer::~Printer()
			{
			//进行正常的析构函数处理;
				--numObjects;
			}
			
			//有问题的代码
			class ColorPrinter: public Printer {
					...
			};
			//现在假设我们系统有一个普通打印机和一个彩色打印机：
			Printer p;
			ColorPrinter cp;
			//当其它对象包含Printer对象时，会发生同样的问题：
			class CPFMachine { // 一种机器，可以复印，打印
				private: // 发传真。
					Printer p; // 有打印能力
					FaxMachine f; // 有传真能力
					CopyMachine c; // 有复印能力
					...
			};
		CPFMachine m1; // 运行正常
		CPFMachine m2; // 抛出 TooManyObjects异常
		
		//你想让一个类产生多个对象 但是不希望被继承
		
		class FSA {
			public:
				// 伪构造函数
				static FSA * makeFSA();
				static FSA * makeFSA(const FSA& rhs);
				...
			private:
				FSA();
				FSA(const FSA& rhs);
				...
	};
		FSA * FSA::makeFSA()
			{ return new FSA(); }
		FSA * FSA::makeFSA(const FSA& rhs)
			{ return new FSA(rhs); }
			
			//为了防止内存泄漏 可以如下调用
			auto_ptr<FSA> pfsa1(FSA::makeFSA());
			// indirectly call FSA copy constructor
			auto_ptr<FSA> pfsa2(FSA::makeFSA(*pfsa1));
			... // 象通常的指针一样使用pfsa1和pfsa2,
					//不过不用操心删除它们。
					
			
			//产生特定多个对象 最多10个
			class Printer {
				public:
					class TooManyObjects{};
					// 伪构造函数
					static Printer * makePrinter();
					static Printer * makePrinter(const Printer& rhs);
					...
				private:
					static size_t numObjects;
					static const size_t maxObjects = 10; // 见下面解释
					Printer();
					Printer(const Printer& rhs);
		};
			// Obligatory definitions of class statics
			size_t Printer::numObjects = 0;
			const size_t Printer::maxObjects;
			Printer::Printer()
			{
				if (numObjects >= maxObjects) {
					throw TooManyObjects();
			}
			...
			}
			Printer::Printer(const Printer& rhs)
			{
				if (numObjects >= maxObjects) {
					throw TooManyObjects();
			}
				...
			}
			Printer * Printer::makePrinter()
				{ return new Printer; }
			Printer * Printer::makePrinter(const Printer& rhs)
				{ return new Printer(rhs); }
			//maxObject也可以这样定义
			class Printer {
				private:
					enum { maxObjects = 10 }; // 在类中,
					... // maxObjects为常量10
			};
		  
		  
		  //一个用来计算对象个数的Base Class
		  template<class BeingCounted>
			class Counted {
				public:
					class TooManyObjects{}; // 用来抛出异常
					static int objectCount() { return numObjects; }
				protected:
					Counted();
					Counted(const Counted& rhs);
					~Counted() { --numObjects; }
				private:
					static int numObjects;
					static const size_t maxObjects;
					void init(); // 避免构造函数的
			}; // 代码重复
		template<class BeingCounted> // 定义numObjects
		int Counted<BeingCounted>::numObjects; // 自动把它初始化为0
			
		template<class BeingCounted>
		Counted<BeingCounted>::Counted()
			{ init(); }
			
		template<class BeingCounted>
		Counted<BeingCounted>::Counted(const Counted<BeingCounted>&)
			{ init(); }
			
		template<class BeingCounted>
		void Counted<BeingCounted>::init()
			{
				if (numObjects >= maxObjects) throw TooManyObjects();
				++numObjects;
			}

	//Printer的写法
		class Printer: private Counted<Printer> {
			public:
			// 伪构造函数
			static Printer * makePrinter();
			static Printer * makePrinter(const Printer& rhs);
			~Printer();
			void submitJob(const PrintJob& job);
			void reset();
			void performSelfTest();
			...
			using Counted<Printer>::objectCount; // 参见下面解释
			using Counted<Printer>::TooManyObjects; // 参见下面解释
		private:
			Printer();
			Printer(const Printer& rhs);
		};
		
		//对于maxObject我们选择让用户定义它
		//Printer的作者必须把这条语句加入到一个实现文件里：
		const size_t Counted<Printer>::maxObjects = 10;