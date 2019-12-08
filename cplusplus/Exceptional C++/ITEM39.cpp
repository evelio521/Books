ITEM39:自动转换
	//标准的string不具有向const char* 进行自动转换的能力。它应该有吗？
	
	//把string做为C风格的const char*来进行访问，经常是很有用的。实际上，string也有一个c_str()
	//专门完成这个任务---该函数返回const char*。下面代码体现两者区别
	string s1("hello"), s2("world"); 
	strcmp( s1, s2 );                // 1 (error) 不存在自动转换
	strcmp( s1.c_str(), s2.c_str() ) // 2 (ok)

  //标准的string不具有向const char* 进行自动转换的能力。它不应该有
  //隐式转换是不安全的
  a)  他会影响重载的解析
  b)他会使错误的代码不声不响的通过编译
  
  //请看下面的例子
  string s1, s2, s3; 
	s1 = s2 - s3;   // oops, probably meant "+" 可能是+，-是无意义的
	             //如果存在隐式转换就不会提示该问题。



