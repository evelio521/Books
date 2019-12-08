规则:13
	/*以by reference 方式捕获异常*/
	//以by pointer方式捕获异常不是好注意 你应该避开他，以by value 或by reference捕获
	
	/*
	 *通过值捕获异常（catch-by-value）可以解决上述的问题，例如异常对象删除的问题和使用标
	 *准异常类型的问题。但是当它们被抛出时系统将对异常对象拷贝两次（参见条款M12）。而且它
	 *会产生slicing problem，即派生类的异常对象被做为基类异常对象捕获时，那它的派生类行为
	 *就被切掉了（sliced off）。这样的sliced对象实际上是一个基类对象：它们没有派生类的数
	 *据成员，而且当本准备调用它们的虚拟函数时，系统解析后调用的却是基类对象的函数。
	 */
	 
	 /*
	  *最后剩下方法就是通过引用捕获异常（catch-by-reference）。通过引用捕获异常能使你避开
	  *上述所有问题。不象通过指针捕获异常，这种方法不会有对象删除的问题而且也能捕获标准异常
	  *类型。也不象通过值捕获异常，这种方法没有slicing problem，而且异常对象只被拷贝一次。
	  */
	  
	  class exception { // 如上，这是
public: // 一个标准异常类
virtual const char * what() throw();
// 返回异常的简短描述.
... // （在函数声明的结尾处
// 的"throw()"，
}; //有关它的信息
// 参见条款14)
			class runtime_error:public exception { ... }; //也来自标准C++异常类
			class Validation_error: // 客户自己加入个类
				public runtime_error {
									public:
										virtual const char * what() throw();
									// 重新定义在异常类中
											... //虚拟函数
				}; //
			void someFunction() // 抛出一个 validation
			{ // 异常
					...
					if (a validation 测试失败) {
					throw Validation_error();
					}
			...
			}
			void doSomething()
			{
					try {
					someFunction(); // 抛出 validation
					} //异常
					catch (exception &ex) { //捕获所有标准异常类
					// 或它的派生类
					cerr << ex.what(); // 调用 Validation_error::what()
					... 
					}
			}