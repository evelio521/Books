ITEM9:等同的代码吗
	//1.Describe what the following code does:
	
	// Example 9-1
	//
	f( a++ );
	
	//Be as complete as you can about all possibilities
	
	//解答
	//若f是下面的一种
	    #define f(x) x                      // once
			#define f(x) (x,x,x,x,x,x,x,x,x)    // 9 times
			#define f(x)                        // not at all
   //设计准则 避免使用宏，宏往往使得代码更难以理解，从而难以维护

	//若是函数
	   
	   //这种情况下，首先，a++会被赋值，结果被传递给函数做参数。通常
	   //后置对象会以临时对象的形式返回a 的旧值，所以f获取参数的方式
	   //要么通过传值，要么通过const引用，但不会以传递非const引用，
	   //因为非const引用不能绑定于临时对象
	 //若是对象
	 
	   //f是一个函数对象，类似于仿函数，f的operator()()可以以传值或const
	   //引用获取参数
	//若是类型名称
	
	   //先对a++求值，赋给f的临时对象
	   
  //若a是宏
     //这种情况下a可以表示任何东西
  //若a是对象(有可能是内建类型)
     //这种情况下必须定义合适的后置递增操作符
     // Canonical form of postincrement:
			T T::operator++(int)
			{
			  T old( *this ); // remember our original value
			
			  ++*this;        // always implement postincrement
			                  //  in terms of preincrement
			
			  return old;     // return our original value
			}
			
			//设计准则:始终为重载的运算符保持正确的语义
  //2.下面两段代码的区别

		// Example 9-2(a)
		//
		f( a++ );
		
		//(1) a++ 递增a并返回a的旧值
		//(2)f()  将a的旧值传递给f(),执行f()
		// Example 9-2(b)
		//
		f( a );
		a++;
		//(1) f() 将a的旧值传递给f(),然后执行f()
		//(2)a++ 递增a并返回a的旧值,旧值随后被忽略
		
		
		//这连个的主要后果
		//(1) f()产生异常的情况下，第一个代码保证a++和他所有的副作用都成功执行结束，而第二个代码保证a++没有执行，他的副作用一个也没发生
		//(2) 即使没有异常产生，如果f()和a.operator++(int)有可见的副作用，那么他们的执行顺序很重要。
		
		//3.假设f()是一个函数，他通过传值的方式获得参数，a是一个类对象，提供了正常语义的operator++(int)
		   //那么代码1和代码2的真正的区别？
		 //对于完全正规的c++，第二段代码不合法，第一段代码合法
		 
		 //如果将将f()换成list::erase()
		 //第一种形式合法
		 // Example 9-3(a)
			//
			// l is a list<int>
			// i is a valid non-end iterator into l
			//
			l.erase( i++ ); // OK, incrementing a valid iterator
			
			//第二段代码不合法:
			
			// Example 9-3(b)
			//
			// l is a list<int>
			// i is a valid non-end iterator into l
			//
			l.erase( i );
			i++;            // error, i is not a valid iterator
			
