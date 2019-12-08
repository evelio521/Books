ITEM44:类型转换
	class  A { public: virtual ~A(); /*...*/ }; 
	A::~A() { }
	
	class B : private virtual A  { /*...*/ };
	
	class C : public  A          { /*...*/ };
	
	class D : public B, public C { /*...*/ };
	
	A a1; B b1; C c1; D d1;
	const A  a2;
	const A& ra1 = a1;
	const A& ra2 = a2;
char c;


	//C++风格的转型
	//具体的见More Effective C++ Rule 2
	//1.const_cast 
  //2.dynamic_cast//不能与C语言相对应
  //3.reinterpret_cast
  //4.static_cast


  //下列每一条语句，用新风格写，哪条不用新风格是不正确的
		 void f() 
		{
		  A* pa; B* pb; C* pc;
		  pa = (A*)&ra1;//pa = const_cast<A*>(&ra1); 
	    pa = (A*)&a2;//这一句无法使用新转型
		  pb = (B*)&c1;// pb = reinterpret_cast<B*>(&c1); 
		  pc = (C*)&d1;//这句在C中是错误的，而C++不需要转型pc = &d1;
		  
		}

   //评判下列每一条C++转型编写风格的正确性
   
   //首先注意，我们并不知道本条款中给出的类时候拥有虚拟函数，如果涉及转型的类
   //不具有虚拟函数，那么对dynamic_cast的转型是错误的
   void g() 
	{
	  unsigned char* puc = static_cast<unsigned char*>(&c);
	  signed char* psc = static_cast<signed char*>(&c);
	  //这两条语句应使用reinterpret_cast ，原因是:char .signed char以及
	  //unsigned char是三个互不相同，区别开来的类型。尽管他们之间存在隐式转换
	  //但是他们之间是互不联系的，当然指针也是互不联系的
	
	  void* pv = static_cast<void*>(&b1);
	  B* pb1 = static_cast<B*>(pv);
	 //这两句都不错，但是第一句的转型是不必要的，本来就有一个对象指针到void*隐式转换
	  B* pb2 = static_cast<B*>(&b1);
	//这句也不错，但是转型是不必要的，其引数已经是一个B*
	  A* pa1 = const_cast<A*>(&ra1);
	  //这一句的是合法的，但是使用转型去掉常量性是潜在不良风格的体现。我们通常可以使用mutable关键字完成
	  A* pa2 = const_cast<A*>(&ra2);
		//错误，如果该指针被用来对对象施行写操作，会产生未定义行为。因为a2是const object，会放在只读存储区
	  B* pb3 = dynamic_cast<B*>(&c1);
		//错误，这一句将pb3设置为NULL，因为c1根本不是一个B对象。
	  A* pa3 = dynamic_cast<A*>(&b1);
		//错误，因为b1不是一个A(B是以private派生与A的)
	  B* pb4 = static_cast<B*>(&d1);
		//不错但是没有必要
	  D* pd = static_cast<D*>(pb4);
		//不错。如果你认为这里需要一个dynamic_cast，那就不对了。因为当目标已知的时候，向下转型可以是静态的。
		pa1 = dynamic_cast<A*>(pb2);
		//错误，你不能使用dynamic_cast将一个指向B对象的指针转换为指向A对象的指针，因为B
		//是以private派生于A
	  pa1 = dynamic_cast<A*>(pb4);
	  //正确
	
	  C* pc1 = dynamic_cast<C*>(pb4);
	  //正确的
	  C& rc1 = dynamic_cast<C&>(*pb2);
	  //错误，pb2并不是真的就是一个C，dynamic_cast可以在指针转型失败时，返回null,但是没有null reference 
	  //这种说法，因此当一个引用类型失败时便无法返回null reference。除了抛出异常，没有别的办法
	  }

