规则:32
	/*在未来的时态下发展程序*/
/*
 *	这么做的一种方法是：用C++语言自己来表达设计上的约束条件，而不是用注释或文档。
 *	例如，如果一个类被设计得不会被继承，不要只是在其头文件中加个注释，用C++的方法
 *	来阻止继承；Item M26显示了这个技巧。如果一个类需要其实例全部创建在堆中，不要只
 *  是对用户说了这么一句，用Item M27的方法来强迫这一点。如果拷贝构造和赋值对一个类
 *  是没有意义的，通过申明它们为私有来阻止这些操作（见Item E27）。C++提供了强大的功能、
 *  灵活度和表达力。用语言提供的这些特性来强迫程序符合设计。
 */
 /*
	*因为万物都会变化，要写能承受软件发展过程中的混乱攻击的类。避免“demand-paged”
  *WQ：“用户要求型”之类的意思吧）的虚函数，凭什么你本没有写虚函数而直到有人来
	*要求后你就更改为虚函数？应该判断一个函数的含意，以及它被派生类重定义的话是否有意义。
	*如果是有意义的，申明它为虚，即使没有人立即重定义它。如果不是的话，申明它为非虚，
	*并且不要在以后为了便于某人而更改；确保更改是对整个类的运行环境和类所表示的抽象是
	*有意义的（见Item E36）。
 */
 /*
	*处理每个类的赋值和拷贝构造函数，即使“从没人这样做过”。他们现在没有这么做并不意
	*味着他们以后不这么做（见Item E18）。如果这些函数是难以实现的，那么申明它们为私有。
	*这样，不会有人误调编译器提供的默认版本而做错事（这在默认赋值和拷贝构造函数上经常发生，见Item E11）。
	
	*要承认：只要是能被人做的，就有人这么做
	
	*努力于可移植的代码。写可移植的代码并不比不可移植的代码难太多，
	*只有在性能极其重要时采用不可移植的结构才是可取的
	
	*将你的代码设计得当需要变化时，影响是局部的。尽可能地封装；将实现细节申明为私有（例子见Item E20）。
	*只要可能，使用无名的命名空间和文件内的静态对象或函数（见Item E31）。避免导致虚基类的设计，
	*因为这种类需要每个派生类都直接初始化它－－即使是那些间接派生类（见Item M4和Item E43）。
	*避免需要RTTI的设计，它需要if...then...else型的瀑布结构（再次参见Item M31，然后看Item E39上的好方法）。
	*每次，类的继承层次变了，每组if...then...else语句都需要更新，如果你忘掉了一个，你不会从编译器得到任何告警。
	*/
	//下列的忠告是不正确的
	1.//你需要虚析构函数，只要有人delete一个实际值向D的B *。
		//这里，B是基类，D是其派生类。换句话说，这位作者暗示，如果你的程序看起来是这样时，并不需要B有虚析构函数：
		class B { ... }; // no virtual dtor needed
		class D: public B { ... };
		B *pb = new D;
		//然而，当你加入这么一句时，情况就变了：
		delete pb; // NOW you need the virtual
		// destructor in B
		//这意味着，用户代码中的一个小变化－－增加了一个delete语句－－实际上能导致需要修改B的定义。
		///如果这发生了的话，所有B的用户都必须重编译。采纳了这个作者的建议的话，一条语句的增加将导致
		//大量代码的重编译和重链接。这绝不是一个高效的设计。
		
	2.//如果一个公有基类没有虚析构函数，所有的派生类基其成员函数都不应该有析构函数。也就是说，这是没问题的：
		class string { // from the standard C++ library
		public:
		~string();
		};
		class B { ... }; // no data members with dtors,
		// no virtual dtor needed
		//但从B继承一个新类后，事情就变了：
		class D: public B {
		string name; // NOW ~B needs to be virtual
		};
		//再一次，一个关于B的使用的小小的变化（这里是增加了一个包含有析构函数的成员对象的派生类）可能需要大
		//量代码的重编译和重链接。但在系统中，小的变化应该只有小的影响。这个设计在这个测试上失败了。
		
	3.//如果多重继承体系有许多析构函数，每个基类都应该有应该虚析构函数。
	4.//我们没有使用虚析构函数，因为我们不想这个string类有vtbl。我们甚至不期望想有一个string *，
	  //所以这不成为问题。我们很清楚这么做将造成的困难
	  
	  //由于string没有virtual destructor，当他派出新类，是一种高度冒险的行为。这个生产商应该提供明确
	  //的文档以指出他的string类没有被设计得可被继承的，但如果程序员没注意到这个警告或未能读到这个文档
	  //时会发生什么？
		//一个可选方法是用C++自己来阻止继承。Item M26描述了怎么限制对象只生成于堆中，以及用auto_ptr对象
		//来操作堆中的对象。构造string对象的接口将不符合传统也不方便，需要这样：
		auto_ptr<String> ps(String::makeString("Future tense C++"));
		... // treat ps as a pointer to
		// a String object, but don't
		// worry about deleting it
		//来代替这个：
		String s("Future tense C++");
		//但，多半，为了减少不正确的继承行为是值得换用不方便的语法的。（对string类，这未必很合算，但对其它类，
		//这样的交换是完全值得的。）
		
		//未来时态的考虑只是简单地增加了一些额外约束：
		
		1.//提供完备的类（见Item E18），即使某些部分现在还没有被使用。如果有了新的需求，你不用回过头去改它们。
		2.//将你的接口设计得便于常见操作并防止常见错误（见Item E46）。使得类容易正确使用而不易用错。例如，
			//阻止拷贝构造和赋值操作，如果它们对这个类没有意义的话（见Item E27）。防止部分赋值（见Item M33）。
		3.//如果没有限制你不能通用化你的代码，那么通用化它。例如，如果在写树的遍历算法，考虑将它通用得可以处
			理任何有向不循环图。