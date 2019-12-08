ITEM40:名字空间。之二:迁徙到名字空间
	
	//假设你在开发一个数百万行代码的项目，其中的.h和.cpp文件有上千个。
	//这时，项目小组要将编译器升级到最新版本，这个版本的编译器支持名字空间，标准
	//库的所有构件也都放在名字空间std中。不幸的是这种顺应标准的做法也有副作用
	//会导致现有代码不能通过编译。而你没有足够的时间去仔细分析每个文件
	//该如何解决这个问题呢？
	
	//安全高效的迁徙到名字空间
	//如今标准库在于名字空间std中，所以在这个项目中，不带修饰的使用std::名称将无法编译成功
	
	//下列代码过去可能通过编译
	// Example 40-1: This used to work
	//
	#include <iostream.h>  // 标准出台前的头文件
	
	int main()
	{
	  cout << "hello, world" << endl;
	}
	//现在，要么你选择写出哪些名称存在于std中
	// Example 40-2(a): Option A, 明确指明一切
	//
	#include <iostream>
	
	int main()
	{
	  std::cout << "hello, world" << std::endl;
	}
	//要么使用using将需要的std名称引入到当前空间
	// Example 40-2(b): Option B, write using-declarations
	//
	#include <iostream>
	
	using std::cout;
	using std::endl;
	
	int main()
	{
	  cout << "hello, world" << endl;
	}
	//要么简单的使用一个using 指令，将所有std名称整个引入到当前空间
	// Example 40-2(c): Option C, write using-directives
	//
	#include <iostream>
	
	int main()
	{
	  using namespace std; // or this can go at file scope
	  cout << "hello, world" << endl;
	}
	//要么，综合以上方法
	
	
	//好的长期方案的设计准则，至少应该遵循一下规则
	//规则1:绝对不要在头文件中使用using指令，using指令会污染名字空间，因为它可能引入
	//大量的名称，其中许多名称是不必要的，不必要名称一旦出现，名字冲突就可能加大。
	//规则2:绝对不要在头文件中使用using声明。
	//规则3:在实现文件中，绝对不要在#include指令之前使用using声明或using指令
	//规则4:在使用C头文件时，采用新风格的#include<cheader> 而不采用旧风格的#include<header.h>
	
	//一个启发的例子
	// Example 40-3(a): 没有名字空间的原始代码
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines Y
	#include <deque.h>
	#include <iosfwd.h>
	
	ostream& operator<<( ostream&, const Y& );
	Y operator+( const Y&, int );
	int f( const deque<int>& );
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines Z
	#include <ostream.h>
	
	ostream& operator<<( ostream& o, const Y& y )
	{
	  // ... uses Z in the implementation ...
	  return o;
	}
	
	Y operator+( const Y& y, int i )
	{
	  // ... uses another operator+() in the implementation...
	  return result;
	}
	
	int f( const deque<int>& d )
	{
	  // ...
	}
	
	//一个好的长期方案
	// Example 40-3(b): A good long-term solution
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines Y
	#include <deque>
	#include <iosfwd>
	
	std::ostream& operator<<( std::ostream&, const Y& );
	Y operator+( const Y&, int i );
	int f( const std::deque<int>& );
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines Z
	#include <ostream>
	using std::deque;   // "using" appears AFTER all #includes
	using std::ostream;
	using std::operator+;
	// or, "using namespace std;" if that suits you
	ostream& operator<<( ostream& o, const Y& y )
	{
	  // ... uses Z in the implementation ...
	  return o;
	}
	
	Y operator+( const Y& y, int i )
	{
	  // ... uses another operator+() in the implementation...
	  return result;
	}
	
	int f( const deque<int>& d )
	{
	  // ...
	}
	
	//一个不那么好的长期方案
	// Example 40-3(c): Bad long-term solution
	// (or, Why to never write using-declarations
	// in headers, even within a namespace)
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines MyProject::Y and adds
	                //  using-declarations/directives
	                //  in namespace MyProject
	#include <deque>
	#include <iosfwd>
	namespace MyProject
	{
	  using std::deque;
	  using std::ostream;
	  // or, "using namespace std;"
	
	ostream& operator<<( ostream&, const Y& );
	Y operator+( const Y&, int );
	int f( const deque<int>& );
	}
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines MyProject::Z and adds
	                //  using-declarations/directives
	                //  in namespace MyProject
	  // error: potential future name ambiguities in
	  //        z.h's declarations, depending on what
	  //        using-declarations exist in headers
	  //        that happen to be #included before z.h
	  //        in any given module (in this case,
	  //        x.h or y.h may cause potential changes
	  //        in meaning)
	#include <ostream>
	
	namespace MyProject
	{
	  using std::operator+;
	
	  ostream& operator<<( ostream& o, const Y& y )
	  {
	    // ... uses Z in the implementation ...
	    return o;
	  }
	
	  Y operator+( const Y& y, int i )
	  {
	    // ... uses another operator+() in the implementation...
	    return result;
	  }
	
	  int f( const deque<int>& d )
	  {
	    // ...
	  }
	}
	//一个有效的短期方案
	//移植步骤1:在每个头文件中，为所有所需要之处添加std::修饰符
	//移植步骤2:创建一个myproject_last.h的新头文件，使之包含using 
	//          namespace std指令。然后，在每一个实现文件中，在所有其他#include
	//          指令之后包含(#include)myproject_last.h
  //使用上面步骤后
  
  // Example 40-3(d): Good short-term solution,
	// applying our two-step migration
	//
	
	//--- file x.h ---
	//
	#include "y.h"  // defines Y
	#include <deque>
	#include <iosfwd>
	
	std::ostream& operator<<( std::ostream&, const Y& );
	  Y operator+( const Y& y, int i );
	int f( const std::deque<int>& );
	
	//--- file x.cpp ---
	//
	#include "x.h"
	#include "z.h"  // defines Z
	#include <ostream>
	#include "myproject_last.h"
	                // AFTER all other #includes
	ostream& operator<<( ostream& o, const Y& y )
	{
	  // ... uses Z in the implementation ...
	  return o;
	}
	
	Y operator+( const Y& y, int i )
	{
	  // ... uses another operator+() in the implementation...
	  return result;
	}
	
	int f( const deque<int>& d )
	{
	  // ...
	}
	
	//--- common file myproject_last.h ---
	//
	using namespace std;
	//这不会累及长期方案，因为他做任何事情都不需要长期方案去撤销
	//同时，和完整的长期方案相比，他更简单，需要的代码修改量更少。实际上
	//想让代码在支持名字空间的编译器上工作，并且不至于以后还得回头去撤销
	//已做的工作，这种方法需要的工作量最少
	
	//最后，很可能以后某个适当的时候，你暂时不再为工程期限所迫，你就可以实施简单的移植策略
	//过渡到40-3(b)所介绍的长期策略。简单遵循以下步骤
	
	//1.在myproject_last.h中，注释掉using指令
	//2.重新编译工程，看看哪些地方不能通过编译，然后在每个实现文件中
	//  增加正确的using指令或using声明
	//3.这一步你可以不做，在每一个头文件或实现文件中，将包含C头文件
	//  代码修改为新的<cheader>形状
		
							
