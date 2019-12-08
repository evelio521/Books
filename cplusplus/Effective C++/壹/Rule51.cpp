����51:
/*��дnew��deleteҪ���س���*/
//һ��operator new��α���룺
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
	
	//ʲôҲûд
};

Derived *p =new Derived;//���õ���Base::operator new

//����base class��operator new����������derived class����д������£���������дbase class��operator new��
//non-menber�汾
void * Base::operator new(std::size_t size) throw(std::bad_alloc)
{
  if (size != sizeof(Base))               // if size is "wrong,"
     return ::operator new(size);         // have standard operator
                                          // new handle the request
  ...                                     // otherwise handle
                                          // the request here
}

//���������size����Ϊ0��operator delete��ΨһҪ����ǡ���֤delete NULL pointer��Զ��ȫ����
void operator delete(void *rawMemory) throw()
{
  if (rawMemory == 0) return;            // do nothing if the null
                                         // pointer is being deleted
  deallocate the memory pointed to by rawMemory;
}
//member�汾
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

