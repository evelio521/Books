规则51:
/*编写new和delete要固守常规*/
//一段operator new的伪代码：
void * operator new(std::size_t size) throw(std::bad_alloc)
{                                      // your operator new might
  using namespace std;                 // take additional params
  if (size == 0) {                     // handle 0-byte requests
    size = 1;                          // by treating them as
  }                                    // 1-byte requests
  while (true) {
   attempt to allocate size bytes;
    if (the allocation was successful)
       return (a pointer to the memory);
    // allocation was unsuccessful; find out what the
    // current new-handling function is (see below)
    new_handler globalHandler = set_new_handler(0);
    set_new_handler(globalHandler);
    if (globalHandler) (*globalHandler)();
    else throw std::bad_alloc();
  }
}


class Base{
	public:
		static void* operator new(std::size_t size) throw(std::bad_alloc);
		...
	
};
class Derived:publicBase{
	
	//什么也没写
};

Derived *p =new Derived;//调用的是Base::operator new

//对于base class的operator new操作，在其derived class不重写的情况下，可以这样写base class的operator new：
//non-menber版本
void * Base::operator new(std::size_t size) throw(std::bad_alloc)
{
  if (size != sizeof(Base))               // if size is "wrong,"
     return ::operator new(size);         // have standard operator
                                          // new handle the request
  ...                                     // otherwise handle
                                          // the request here
}

//独立对象的size不会为0。operator delete的唯一要求就是“保证delete NULL pointer永远安全”：
void operator delete(void *rawMemory) throw()
{
  if (rawMemory == 0) return;            // do nothing if the null
                                         // pointer is being deleted
  deallocate the memory pointed to by rawMemory;
}
//member版本
class Base{
	public:
		static void* operator new(std::size_t size) throw(std::bad_alloc);
		static void* operator delete(void* raeMemory,std::size_t size) throw();
		...
	
};
void Base::operator delete(void *rawMemory, std::size_t size) throw()
{
  if (rawMemory == 0) return;           // check for null pointer
  if (size != sizeof(Base)) {           // if size is "wrong,"
     ::operator delete(rawMemory);      // have standard operator
     return;                            // delete handle the request
  }

