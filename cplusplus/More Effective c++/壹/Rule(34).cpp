规则34:
	/*如何在同一个程序中结合c++和c*/
	
	/*
	 * 确认兼容后，还有四个要考虑的问题：
	 * 1.名变换
	 * 2.静态初始化
	 * 3.内存动态分配
	 * 4.数据结构兼容
	 */
	 
	 //1.名称重整	 
	 /*
	  *编译时，c++由于存在重载。所以函数会被编译器给予另外一个名字，
	  *而c不存在这些情况，你给定的函数是什么名字就是什么名字，编译器
	  *不会改变函数的名字。所以要使c和c++有兼容性，必须抑制c++对函数
	  *名字改变。压抑名字重整的做法：
	  */
	  // declare a function called drawLine; don't mangle
		// its name
		extern "C"
		void drawLine(int x1, int y1, int x2, int y2);
		
		//压抑一组不需要重整的名字
		extern "C" { // disable name mangling for
		// all the following functions
		void drawLine(int x1, int y1, int x2, int y2);
		void twiddleBits(unsigned char bits);
		void simulate(int iterations);
		...
		}
		
		//在c++中压抑名字重整，在c中不需要
		//当用C++编译时，你应该加extern 'C'，但用C编译时，不应该这样。
		//通过只在C++编译器下定义的宏__cplusplus，你可以将头文件组织得这样：
		#ifdef __cplusplus
		extern "C" {
		#endif
		void drawLine(int x1, int y1, int x2, int y2);
		void twiddleBits(unsigned char bits);
		void simulate(int iterations);
		...
		#ifdef __cplusplus
		}
		#endif
		
		//2.statics的初始化
		
		/*
		 *在掌握了名变换后，你需要面对一个C++中事实：在main执行前和执行
		 *后都有大量代码被执行。尤其是，静态的类对象和定义在全局的、命名
		 *空间中的或文件体中的类对象的构造函数通常在main被执行前就被调用。
		 *这个过程称为静态初始化（参见Item E47）。这和我们对C++和C程序的
		 *通常认识相反，我们一直把main当作程序的入口。同样，通过静态初始
		 *化产生的对象也要在静态析构过程中调用其析构函数；这个过程通常发
		 *生在main结束运行之后。
		 */
		 
		 //为了解决main()应该首先被调用，而对象又需要在main()执行前被构造
		 //的两难问题，许多编译器在main()的最开始处插入了一个特别的函数，
		 //由它来负责静态初始化。同样地，编译器在main()结束处插入了一个函
		 //数来析构静态对象。产生的代码通常看起来象这样：
			int main(int argc, char *argv[])
			{
			performStaticInitialization(); // generated by the
			// implementation
			the statements you put in main go here;
			performStaticDestruction(); // generated by the
			// implementation
			}
			
			//有时看起来用C写main()更有意义－－比如程序的大部分是C的，C++部分
			//只是一个支持库。然而，这个C++库很可能含有静态对象（即使现在没有，
			//以后可能会有－－参见Item M32），所以用C++写main()仍然是个好主意。
			//这并不意味着你需要重写你的C代码。只要将C写的main()改名为realMain()，
			//然后用C++版本的main()调用realMain()：
			extern "C" // implement this
			int realMain(int argc, char *argv[]); // function in C
			int main(int argc, char *argv[]) // write this in C++
			{
				return realMain(argc, argv);
			}
			
			//3.动态内存分配
			
			/*
			 * C++部分使用new和delete（参见Item M8），C部分使用malloc（或其变形）和free。
			 * 只要new分配的内存使用delete释放，malloc分配的内存用free释放，那么就没问题。
			 * 用free释放new分配的内存或用delete释放malloc分配的内存，其行为没有定义。
			 * 那么，唯一要记住的就是：将你的new和delete与mallco和free进行严格的隔离。
			 */
			//4.数据结构兼容性
			/*
			 *因为C++中的struct的规则兼容了C中的规则，假设“在两类编译器下定义的同一结构
			 *将按同样的方式进行处理”是安全的。这样的结构可以在C++和C见安全地来回传递。如
       *果你在C++版本中增加了非虚函数，其内存结构没有改变，所以，只有非虚函数的结构（或类）
       *的对象兼容于它们在C中的孪生版本（其定义只是去掉了这些成员函数的申明）。增加虚
       *函数将结束游戏，因为其对象将使用一个不同的内存结构（参见Item M24）。
       *从其它结构（或类）进行继承的结构，通常也改变其内存结构，所以有基类的结构也不能与C函数交互。
       */
       
       //总结
       如果想在同一程序下混合C++与C编程，记住下面的指导原则：
			1.确保C++和C编译器产生兼容的obj文件。
			2.将在两种语言下都使用的函数申明为extern 'C'。
			3.只要可能，用C++写main()。
			4.总用delete释放new分配的内存；总用free释放malloc分配的内存。
			5.将在两种语言间传递的东西限制在用C编译的数据结构的范围内；这些结构的C++版本可以包含非虚成员函数。