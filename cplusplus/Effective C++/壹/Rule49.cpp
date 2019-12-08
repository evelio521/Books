����:49
/*�˽�new-handler����Ϊ*/

//set_new_handler�Ĳ����Ǹ�ָ�룬ָ��operator new�޷������㹻���ڴ�ʱ�ñ����õĺ�����
//�䷵��ֵҲ�Ǹ�ָ�룬ָ��set_new_handler������ǰ����ִ�е��Ǹ�new-handler����

//set_new_handler���÷�
#include<new>
#include<iostream>
using namespace std;
void outMem(){
	
	cerr<<"�ڴ����ʧ��";
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

//new_handler��operator new��������ͨ���б�ʵ�֣�Ϊ��ʹ�÷������Ա�����ʹ�ã����Խ���ģ�廯��
template<typename T>              // "mixin-style" base class for
class NewHandlerSupport{          // class-specific set_new_handler
public:                           // support
  static std::new_handler set_new_handler(std::new_handler p) throw();
  static void * operator new(std::size_t size) throw(std::bad_alloc);
  ...                             // other versions of op. new ��
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
//��Ҫ��new_handler��operator new�ر������ֱ�Ӵ�NewHandlerSupport�������ɣ�
class Widget: public NewHandlerSupport<Widget> {
  ...                          // as before, but without declarations for
};                             // set_new_handler or operator new
//��һ�֡����׳��쳣����operator new��ʵ�֣�
class Widget { ... };
Widget *pw1 = new Widget;                 // throws bad_alloc if
                                          // allocation fails
if (pw1 == 0) ...                         // this test must fail
Widget *pw2 =new (std::nothrow) Widget;   // returns 0 if allocation for
                                          // the Widget fails
if (pw2 == 0) ...                         // this test may succeed
//��Ҫע����ǣ���������ֻ�ܱ�֤pw2��ִ��Widget *pw2 =new (std::nothrow) Widgetʱ���׳��쳣��
//ȴ���ܱ�֤pw2�е�operator new�������׳��쳣��
