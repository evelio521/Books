ITEM47:控制流
	//从下面的代码中找出控制流的毛病
	#include <cassert> 
	#include <iostream>
	#include <typeinfo>
	#include <string>
	using namespace std;
	
	//  The following lines come from other header files.
	//全局变量的出现应该早引起我们的警觉，使我们特别留意那些企图在它被初始化之前就使用它的代码
	//在哥翻译单元之间的那些全局变量(包括类的静态变量)的初始化顺序未定义
	char* itoa( int value, char* workArea, int radix );
	extern int fileIdCounter;
	//
	// 使类的不变量检查自动化的辅助函数
	//
	template<class T>
	inline void AAssert( T& p )
	{
	  static int localFileId = ++fileIdCounter;
	  //这里出问题了，如果编译器恰好在初始化任何AAssert<T>::localFileIdCounter进行了初始化，那么还算好，
	  //否则的话哲理的数值将会是fileIdCounter在初始化前其所占用的内存区中的内容
	  if( !p.Invariant() )
	  {
	    cerr << "Invariant failed: file " << localFileId
	         << ", " << typeid(p).name()
	         << " at " << static_cast<void*>(&p) << endl;
	    assert( false );
	  }
	}
	
	template<class T>
	class AInvariant
	{
	public:
	  AInvariant( T& p ) : p_(p) { AAssert( p_ ); }
	  ~AInvariant()              { AAssert( p_ ); }
	private:
	  T& p_;
	};
	#define AINVARIANT_GUARD AInvariant<AIType> invariantChecker( *this )
	//使用这些辅助函数是个很有意思的主意，这样可以使一个类在函数调用的前后自动进行不变量的检查。
	//只要简单的对其一个AItype的typedef,然后再把AINVARIANT_GUARD做为成员函数的第一条语句就可以了
	//本质上这样不安全是不好的。
	
	//然而在下面的代码中，这种做法就很不幸的变得一点都不有趣。主要原因是Ainvariant隐藏了对assert()的
	//调用，而当持续在non-debug模式下被建立的时候，编译器会自动的删除掉assert()。
	//-------------------------------------------------------------
	template<class T>
	class Array : private ArrayBase, public Container
	{
	  typedef Array AIType;
	public:
	  Array( size_t startingSize = 10 )
	  : Container( startingSize ),
	    ArrayBase( Container::GetType() ),
	    	//这里构造函数的初始化表有两个潜在错误。第一个错误或许不必称其为错误，但让其
	    	//留在代码里面又会形成一个扑簌迷离的障眼法。我们分两点说这个错误:
	    	//1.如果GetType()是一个静态成员函数，或者是一个既不使用this指针又不受构造操
	    	//作副作用(静态的使用计数)影响的成员函数，那么这里只是不良的编码风格而已，仍然能正确运行
	    	//2.否则的话，我们就有麻烦了，非virtual的基类会被从左到右按照他们被声明的顺序初始化
	    	//因此这里，ArrayBase会在Container之前初始化。不幸的是，这意味着企图使用一个尚未初始化的Container subobject值成员
	    	
	    	
	    used_(0),
	    size_(startingSize),
	    buffer_(new T[size_])
	    //变量是以类定义的顺序初始化
	  {
	    AINVARIANT_GUARD;
	    //效率方面的小问题，在这里Invariant()函数没有必要被调用2次，这是个小问题不会引起大的麻烦。
	  }
		
	  void Resize( size_t newSize )
	  {//这是个严重的控制流方面的问题。
	  	//代码不是异常安全的。如果对new[]的调用导致抛出一个异常的话，那么不但当前的对象会处在一个
	  	//无效的状态，而且原来的buffer还会出现内存泄漏的情况。因为所有指向他的指针都丢失了从而导致不能将其删除。
	    AINVARIANT_GUARD;
	    T* oldBuffer = buffer_;
	    buffer_ = new T[newSize];
	    copy( oldBuffer, oldBuffer+min(size_,newSize), buffer_ );
	    delete[] oldBuffer;
	    size_ = newSize;
	  }
	
	  string PrintSizes()
	  {
	  	//其中itoa()原型函数使用buf做为存放结果的地方。这段代码也有控制流方面的问题。
	  	//我们无法估计最后那个返回语句中对表达式的求职顺序，因为对函数参数的操作顺序是
	  	//没有明确规定的，其完全取决于特定的实现方案。
	  	//最后那个返回语句所表现出来的问题的严重性在下面的这个语句得到更好的展示
	  	//return 
			  //operator+(
			    //operator+(
			     // operator+( string("size = "),
			                // itoa(size_,buf,10) ) ,
			     // ", used = " ) ,
			    //itoa(used_,buf,10) );

				//这里我们假设size_的值为10，used_的值为5.如果最外面的operator+()的第一个参数先被求值的话
				//那么结果将会是正确的size=10,used=5,因为第一个itoa()函数存放在buf里面的结果会在第二个itoa()
				//函数复用buf之前就读出来使用。但如果最外面的operator+()的第二个参数先被求值的话，那么结果回事错误的
				//size=10,used=10,因为外层的那个itoa()函数先被求值，但其结果会在被使用之前就又被内层那个itoa()函数毁掉了
	    AINVARIANT_GUARD;
	    char buf[30];
	    return string("size = ") + itoa(size_,buf,10) +
	           ", used = " + itoa(used_,buf,10);
	  }
	
	  bool Invariant()
	  {
	  	//对Resize()的调用存在两个问题
	  	//1.这种情况，程序压根就不会正常工作，因为如果条件判断为真，那么Resize()会被调用，这又会导致立即
	  	//再次调用Invarient()，接着条件判断仍然会为真，然后在调用Resize()，这又会导致立即再次被调用Invarient()
	  	//，接着...你明白了。这是个无法终止的递归调用
	  	//2.如果AAssert()编写者处于效率方面考虑的错误报告的代码删除掉并取而代之以assert(p->Invarient())
	  	//那又会如何？其结果只会是这里的代码变得更可悲，因为在assert()调用中加入了会产生副作用的代码
	  	//这意味着程序在debug mode和release mode两种不同的编译模式下产生的可执行代码在执行时会有不同行为。即使
	  	//没有上面第一点中说明的问题，这也是很不好的，这意味着着Array对象会依据建立模式的不同而Resize()不同的次数，
	  	//使软件测试人员过上地狱般的生活
	  	
	  	//绝不要在对assert()的调用中加入有副作用的代码，并且总是确认递归肯定互终止
	    if( used_ > 0.9*size_ ) Resize( 2*size_ );
	    return used_ <= size_;
	  }
	private:
	  T*     buffer_;
	  size_t used_, size_;
	};
	
	int f( int& x, int y = x ) { return x += y; }
	//那第二个设置缺省值的参数无论如何都不算是一个合法的C++用法，再一个理想的编译器下
	//应该编译不通过。说这个用法不好还是因为编译器可以采用任意的顺序对函数参数求值，y
	//可能赶在x之前先被初始化。
	int g( int& x )            { return x /= 2; }
	
	int main( int, char*[] )
	{
	  int i = 42;
	  cout << "f(" << i << ") = " << f(i) << ", "
	       << "g(" << i << ") = " << g(i) << endl;
	  //这里还是对参数求值的顺序问题。由于没有确定f(i)和g(i)被求值的先后顺序，
	  //因此显示出来的结果可能是错误的。不同的编译器可能会有不同的结果。
	  Array<char> a(20);
	  cout << a.PrintSizes() << endl;
	}

