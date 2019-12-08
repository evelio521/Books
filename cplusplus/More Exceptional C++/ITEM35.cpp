ITEM35:宏定义
	//1.示范如何写一个简单的预处理宏max(),这个宏有两个参数，并通过普通的<元算
	//他比较出其中的较大值，在写这样一个宏，一般会有哪写易犯错误。
	
	//1.不要忘记为参数加上括号
	// Example 35-1(a): Paren pitfall #1: arguments
	//
	#define max(a,b) a < b ? b : a
  //我们写
  max( i += 3, j )
	//展开后
	i += 3 < j ? j : i += 3
	//上面代码的实际运算顺序为
	i += ((3 < j) ? j : i += 3)
  
  //2.不要忘记为整个展开式加上括号
  // Example 35-1(b): Paren pitfall #2: expansion
	//
	#define max(a,b) (a) < (b) ? (b) : (a)
	
	 //我们写
  k = max( i, j ) + 42;
	//展开后
	k = (i) < (j) ? (j) : (i) + 42;
	//上面代码的实际运算顺序为
	k = (((i) < (j)) ? (j) : ((i) + 42));
	
	//3.当心多余参数运算
	// Example 35-1(c): Multiple argument evaluation
	//
	#define max(a,b) ((a) < (b) ? (b) : (a))
	//我们写
	max( ++i, j )
	//实际为
	((++i) < (j) ? (j) : (++i))

	//我们写
	max( f(), pi )
  //实际为
  ((f()) < (pi) ? (pi) : (f()))

	//4.名字冲突
	// Example 35-1(d): Name tromping
	//
	#define max(a,b) ((a) < (b) ? (b) : (a))
	
	#include <algorithm> // oops!
	//问题在于头文件中有下面代码的东西时
	template<typename T> const T&
	max(const T& a, const T& b);
	//于是宏就将源代码替换的一团糟
	template<typename T> const T&
  ((const T& a) < (const T& b) ? (const T& b) : (const T& a));
  //5.宏不能递归
  //6.宏没有地址
  //7.宏有碍测试








	

  
