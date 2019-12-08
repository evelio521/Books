ITEM37:前置声明
	//1.前置声明是非常有用的工具。但是在这个例子中，它没有像程序员预计的那样工作，做标记的
	//那两行代码为什么是错误的？
	// file f.h
	//
	class ostream;  // error
	class string;   // error
	string f( const ostream& );
	
  //错误在于，你不能以这种方式前置声明ostream和string,
  //因为他们不是类，他们是模版typedef
  
  //2.不包含任何其他文件，为上面的ostream和string写出正确的前置声明
  
  //答案是不可能写出的。实际情况是:不存在某种标准且具有可移植性的方法，
  //可以做到不包含另一个文件却能前置声明ostream,根本没有一种标准且具有
  //可移植性的方法可以前置声明string
  
  //我们不能对namespace std写出我们自己的声明
  
  //你能做到最好的只能像下面这样
  #include <iosfwd>
	#include <string>
	
	//设计准则: 当前置声明可以满足需要时，绝对不要包含#include头文件，在不需要流的完整定义时
	//尽量只包含#include<iosfwd>


  