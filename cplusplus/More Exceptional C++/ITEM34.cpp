ITEM34:预处理宏
	//我们使用宏的理由
	
	//1.守护头文件
	
	//为了防止头文件多次包含，这是一种技巧
	#ifndef MYPROG_X_H
	#define MYPROG_X_H
	// ... the rest of the header file x.h goes here...
	#endif

  //2.使用预处理特性
  //在诊断代码中，插入行号或编译时间这类信息通常很有用。要做到这一点，
  //一个简单的方法是使用预定义标准宏，如__FILE__, __LINE__, __DATE__, 和__TIME__. 
  //基于相同原因，以及其他原因，使用stringizing(字符串化)和token-pasting(标记合并)预处理运算符也很有用。
  
  //3.在编译时期选择代码
    //A 调试代码
      //在编译你的系统时，有时候你想使用某些额外代码，但有时候你又不想这样做
       void f()
			{//这段实际上是两段不同的代码
			#ifdef MY_DEBUG
			  cerr << "some trace logging" << endl;
			#endif
			  // ... the rest of f() goes here...
			}
			//用条件表达式代替这个#define会更好
			void f()
		{
		  if( MY_DEBUG )
		  {
		    cerr << "some trace logging" << endl;
		  }
		  // ... the rest of f() goes here...
		}
		//B 特定平台代码
		//通常在处理针对特定平台的代码时，最好运用factory模式，采用这种方法，代码的组合会更合理
		//运行时期会更具有灵活性。但有时，由于存在的差异太少，你很难构造一个合理的factory，这时候
		//预处理是一种切换可选代码的方法
		
		//C 不同的数据表示方式
		//一个常见的例子是:对于一个模块所定义的一组错误代码，外部用户看到的应该是
		//一个简单的enum,并带有解释。但在模块内部，他们应该被存在一个map中，便于查找
		// For outsiders
		//
		enum Error
		{
		  ERR_OK = 0,            // No error
		  ERR_INVALID_PARAM = 1, // <description>
		  ...
		};
		
		// For the module's internal use
		//
		map<Error,const char*> lookup;
		lookup.insert( make_pair( ERR_OK,
		                          (const char*)"No error" ) );
		lookup.insert( make_pair( ERR_INVALID_PARAM,
		                          (const char*)"<description>" ) );
		...
		//我们想同时拥有两种表达方式，但不希望将实际信息定义两次，有了宏这一魔法
		//我们就可以像下面这样，简单的写一个错误列表，在编译时期创建相应的数据结构
		
		ERR_ENTRY( ERR_OK,            0, "No error" ),
		ERR_ENTRY( ERR_INVALID_PARAM, 1, "<description>" ),
		...

		
		//设计准则:处理以下情况外，避免使用预处理宏
		  //1.守护头文件
		  //2.条件编译，以获取可移植性，或在.cpp文件中进行测试
		  //3.用#pragma禁止无伤大雅的警告，但是这种#pragma总得包含一个为了获得可移植性而提供的
		     //条件编译之中，防止编译器不认识他们而发出警告
		
		
		
		
