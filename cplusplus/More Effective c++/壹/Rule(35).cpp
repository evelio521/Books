规则:35
	/*让自己习惯使用标准C++语言*/
	
	//ISO/ANSI C++标准是厂商实现编译器时将要考虑的，是作者们准备出书时将要分析的，
	//是程序员们在对C++发生疑问时用来寻找权威答案的。在《ARM》之后发生的最主要变化是以下内容：
		1.增加了新的特性：RTTI、命名空间、bool，关键字mutable和explicit，
		  对枚举的重载操作，已及在类的定义中初始化const static成员变量。
		2.模板被扩展了：现在允许了成员模板，增加了强迫模板实例化的语法，
		  模板函数允许无类型参数，模板类可以将它们自己作为模板参数。
		3.异常处理被细化了：异常规格申明在编译期被进行更严格的检查，
		  unexpected()函数现在可以抛一个bad_exception对象了。
		4.内存分配函数被改良了：增加了operator new[]和operator delete[]函数， 
		  operator new/new[]在内存分配失败时将抛出一个异常，并有一个返回为0（不抛
		  异常）的版本供选择。（见Effective C++ Item 7）
		5.增加了新的类型转换形式：static_cast、dynamic_cast、const_cast，和reinterpret_cast。
		6.语言规则进行了重定义：重定义一个虚函数时，其返回值不需要完全的匹配了
		 （如果原来返回基类对象或指针或引用，派生类可以返回派生类的对象、指针或引用），
		  临时对象的生存期进行了精确地定义。
		
		//STL
		1.Container
		2.Iterator
		3.Algorithm