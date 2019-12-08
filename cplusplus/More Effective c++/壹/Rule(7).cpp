规则7:
	/*ItemM7：不要重载“&&”,“||”, 或“,”*/
	//如果你重载了操作符&&，对于你来说代码是这样的：
	if (expression1 && expression2) ...
  //对于编译器来说，等同于下面代码之一：
	if (expression1.operator&&(expression2)) ...
								// when operator&& is a
								// member function
	if (operator&&(expression1, expression2)) ...
								// when operator&& is a
								// global function
	/* 
	 * 以上调用可以导致的结果:
	 * 首先当函数被调用时，需要运算其所有参数，所以调用函
	 * 数functions operator&& 和 operator||时，两个参数
	 * 都需要计算，换言之，没有采用短路计算法。第二是C++语
	 * 言规范没有定义函数参数的计算顺序，所以没有办法知道表
	 * 达式1与表达式2哪一个先计算。完全可能与具有从左参数到
	 * 右参数计算顺序的短路计算法相反。						
	 */
	 
	 /*
	  * 对于内建类型&&和||，C++有一些规则来定义它们如何运算。
	  * 与此相同，也有规则来定义逗号操作符的计算方法。一个包
	  * 含逗号的表达式首先计算逗号左边的表达式，然后计算逗号
	  * 右边的表达式；整个表达式的结果是逗号右边表达式的值。
	  * 所以在上述循环的最后部分里，编译器首先计算++i，然后是
	  * —j，逗号表达式的结果是--j。
	  * 如果你重载逗号表达式将无法提供这样的效果
	  * /
	  
	  /*
	   * 你不能重载下面的操作符：
     *   .    .*        ::        ?:
     *  new  delete    sizeof    typeid
     *  static_cast dynamic_cast const_cast reinterpret_cast
     */
     
     /*
	   * 你能重载下面的操作符：
     *  operator new operator delete
		 *  operator new[] operator delete[]
     *  + - * / % ^ & | ~
     *  ! = < > += -= *= /= %=
     *  ^= &= |= << >> >>= <<= == !=
     *  <= >= && || ++ -- , ->* ->
     *  () []
     */
     */

	