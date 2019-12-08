规则8:
	/*理解各种不同含义的new和delete*/
	string *ps = new string("Memory Management");
	//你使用的new是new操作符。这个操作符就象sizeof一样
	//是语言内置的，你不能改变它的含义，它的功能总是一样的。
	//它要完成的功能分成两部分。第一部分是分配足够的内存以
	//便容纳所需类型的对象。第二部分是它调用构造函数初始化内
	//存中的对象。new操作符总是做这两件事情，你不能以任何方式改变它的行为。
	
	//函数operator new 通常这样声明：
	void * operator new(size_t size);
	//返回值类型是void*，因为这个函数返回一个未经处理（raw）的指针，未初始化的内存。
	
	void *rawMemory = operator new(sizeof(string));
	//操作符operator new将返回一个指针，指向一块足够容纳一个string类型对象的内存。
	//就象malloc一样，operator new的职责只是分配内存。它对构造函数一无所知。
	//operator new所了解的是内存分配。把operator new 返回的未经处理的指针传
	//递给一个对象是new操作符的工作。
	
	string *ps = new string("Memory Management");
	//一下代码的功能和上面的一样
	void *memory = // 得到未经处理的内存
	operator new(sizeof(string)); // 为String对象
	call string::string("Memory Management") //初始化
	on *memory; // 内存中的对象
	string *ps =static_cast<string*>(memory); // 是ps指针指向新的对象
	
	//placement new 的用法 #include<new>
	class Widget {
		public:
			Widget(int widgetSize);
			...
	};
	Widget * constructWidgetInBuffer(void *buffer,int widgetSize)
	{
		return new (buffer) Widget(widgetSize);
	}
	
	
	//删除与内存释放
	string *ps;
	...
	delete ps; // 使用delete 操作符
	
	void *buffer = operator new(50*sizeof(char));// 分配足够的 内存以容纳50个char
                                             //没有调用构造函数
  ...
  operator delete(buffer); // 释放内存 没有调用析构函数
  
  //placement new释放内存的方法
  /*
   * 如果你用placement new在内存中建立对象，你应该避免在该内存
   * 中用delete操作符。因为delete操作符调用operator delete来释
   * 放内存，但是包含对象的内存最初不是被operator new分配的，
   * placement new只是返回转递给它的指针。谁知道这个指针来自何方？
   * 而你应该显式调用对象的析构函数来解除构造函数的影响：
   */
    // 在共享内存中分配和释放内存的函数
    void * mallocShared(size_t size);
		void freeShared(void *memory);
		void *sharedMemory = mallocShared(sizeof(Widget));
		Widget *pw = // 如上所示,
		constructWidgetInBuffer(sharedMemory, 10); // 使用// placement new
		...
		delete pw; // 结果不确定! 共享内存来自 mallocShared, 而不是operator new
		pw->~Widget(); // 正确。 析构 pw指向的Widget，但是没有释放包含Widget的内存
		freeShared(pw); // 正确。 释放pw指向的共享内存但是没有调用析构函数
		
		//数组资源的释放
		string *ps=new string[10];
		
		delete [] ps;