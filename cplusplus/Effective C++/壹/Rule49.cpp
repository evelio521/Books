规则:49
/*了解new-handler的行为*/

//set_new_handler的参数是个指针，指向operator new无法分配足够的内存时该被调用的函数。
//其返回值也是个指针，指向set_new_handler被调用前正在执行的那个new-handler函数

//set_new_handler的用法
#include<new>
#include<iostream>
using namespace std;
void outMem(){
	
	cerr<<"内存分配失败";
	abort();
	
}

int main(){
	set_new_handler(outMem);
	try
   {
      while ( 1 ) 
      {
         new int[5000000];
         cout << "Allocating 5000000 ints." << endl;
      }
   }
   catch ( exception e )
   {
      cout << e.what( ) << " xxx" << endl;
   }
	
}

//new_handler和operator new可以在普通类中被实现，为了使该方案可以被复合使用，可以将其模板化：
template<typename T>              // "mixin-style" base class for
class NewHandlerSupport{          // class-specific set_new_handler
public:                           // support
  static std::new_handler set_new_handler(std::new_handler p) throw();
  static void * operator new(std::size_t size) throw(std::bad_alloc);
  ...                             // other versions of op. new —
                                  // see Item 52
private:
  static std::new_handler currentHandler;
};
template<typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
std::new_handler oldHandler = currentHandler;
currentHandler = p;
return oldHandler;
}
template<typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size)
  throw(std::bad_alloc)
{
  NewHandlerHolder h(std::set_new_handler(currentHandler));
  return ::operator new(size);
}
// this initializes each currentHandler to null
template<typename T> std::new_handler NewHandlerSupport<T>::currentHandler = 0;
//需要将new_handler和operator new特别处理的类直接从NewHandlerSupport派生即可：
class Widget: public NewHandlerSupport<Widget> {
  ...                          // as before, but without declarations for
};                             // set_new_handler or operator new
//有一种“不抛出异常”的operator new的实现：
class Widget { ... };
Widget *pw1 = new Widget;                 // throws bad_alloc if
                                          // allocation fails
if (pw1 == 0) ...                         // this test must fail
Widget *pw2 =new (std::nothrow) Widget;   // returns 0 if allocation for
                                          // the Widget fails
if (pw2 == 0) ...                         // this test may succeed
//需要注意的是，上述代码只能保证pw2在执行Widget *pw2 =new (std::nothrow) Widget时不抛出异常，
//却不能保证pw2中的operator new操作不抛出异常。
