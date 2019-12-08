ITEM31:名字搜索
	//在下面的代码中，调用的是哪个函数？为什么？
			namespace A 
		{
		  struct X;
		  struct Y;
		  void f( int );
		  void g( X );
		}
		
		namespace B
		{
		  void f( int i )
		  {
		  	//这个f()调用的是它自己，并且是无穷递归，在namespace A 也有一个f(int)函数
		  	//如果B写了using namespace A或 using A::f,那么A:f将是寻找f(int)的候选函数
		  	//那么f(i)的调用将存在二义性，由于B没有引入，所以不存在二义性
		    f( i );   // which f()?
		  }
		  void g( A::X x )
		  {
		  	//这个调用存在二义性，程序员必须使用命名空间确定使用哪个函数
		  	//规则:如果你给函数提供一个class型的实参(此处时A::X的类型x)
		  	//那么在名称收索时，编译器将认为包含实参类型的命名空间中的同名函数是
		  	//候选函数
		    g( x );   // which g()?
		  }
		  void h( A::Y y )
		  {
		  	//没有其他的h(A::Y)函数，所以在这f()调用的是他自己，也是无穷递归
		  	//虽然B::h()原型使用namespace A 的一个类型，但是不影响收索结果
		  	//namespace A没有符合h(A::Y)名称的函数
		    h( y );   // which h()?
		  }
		}
		
