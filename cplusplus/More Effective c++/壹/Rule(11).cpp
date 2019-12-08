规则:11
	 /*禁止异常信息（exceptions）传递到析构函数外*/
	 class Session {
		public:
			Session();
			~Session();
			...
		private:
			static void logCreation(Session *objAddr);
			static void logDestruction(Session *objAddr);
		};
		//函数logCreation 和 logDestruction被分别用于记录对象的建立与释放。我们因此可以这样编写Session的析构函数：
		Session::~Session()
		{
				logDestruction(this);
		}
		/*
		 *一切看上去很好，但是如果logDestruction抛出一个异常，会发生什么事呢？
		 *异常没有被Session的析构函数捕获住，所以它被传递到析构函数的调用者那里。
		 *但是如果析构函数本身的调用就是源自于某些其它异常的抛出，那么terminate
		 *函数将被自动调用，彻底终止你的程序。这不是你所希望发生的事情。程序没有
		 *记录下释放对象的信息，这是不幸的，甚至是一个大麻烦。那么事态果真严重到了
		 *必须终止程序运行的地步了么？如果没有，你必须防止在logDestruction内抛出的
		 *异常传递到Session析构函数的外面。唯一的方法是用try和catch blocks
		 */
		 
		 Session::~Session()
			{
				try {
					logDestruction(this);
				}
				catch (...) {
					cerr << "Unable to log destruction of Session object "
								<< "at address "
								<< this
								<< ".\n";
				}
			}
			
			/*
			 *但是这样做并不比你原来的代码安全。如果在catch中调用operator<<时导致
			 *一个异常被抛出，我们就又遇到了老问题，一个异常被转递到Session析构函数的外面。
			 *我们可以在catch中放入try，但是这总得有一个限度，否则会陷入循环。因此我们在释
			 *放Session时必须忽略掉所有它抛出的异常：
			 */
			Session::~Session()
			{
			try {
			logDestruction(this);
			}
			catch (...) { }
			}
			/* catch表面上好像没有做任何事情，这是一个假象，实际上它阻止了任何从logDestruction
			 *抛出的异常被传递到session析构函数的外面。我们现在能高枕无忧了，无论session对象是不
			 *是在堆栈退栈（stack unwinding）中被释放，terminate函数都不会被调用。
			 */