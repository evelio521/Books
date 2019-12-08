规则:52
/*写了placement new 也要写placement delete*/
//operator new和operator delete要成对使用，不仅仅是指形式上要搭配，在本质上new和delete也要是对应的

// normal forms of new & delete
void* operator new(std::size_t) throw(std::bad_alloc);
void operator delete(void *rawMemory) throw();  // normal signature at global scope
void operator delete(void *rawMemory,std::size_t size) throw();  // normal signature at class scope

// placement version of new & delete
void* operator new(std::size_t, std::ostream& logStream) throw(std::bad_alloc);   // "placement new"
void operator delete(void *rawMemory, std::ostream&) throw();// "placement delete"

//规则：如果一个带额外参数的operator new没有带额外参数的对应版本的operator delete ，
//那么内存分配动作需要取消并恢复旧观时就没有任何operator delete会被调用

class Widget{
	
	public:
		...
		static void* operator new(std::size_t size,std::ostream& logstream) throw(std::bad_alloc);// "placement new"
			static void operator delete(void *pMemory) throw();//normal signature 
			static void operator delete(void *rawMemory, std::ostream&logStream) throw();// "placement delete"
				...
					
};
Widget *pw=new(std::cerr) Widget;//不会内存泄漏
delete pw;//调用正常的operator delete


//防止作用域名字遮掩问题
class StandardNewDeleteForm{
	//内含所有的正常形式的new和delete
	
	//normal new/delete
	static void* operator new(std::size_t size) throw(std::bad_alloc);
		{
			return ::operator new(size);	
		}
	static void operator delete(void *rawMemory) throw(); 
		{
		return ::operator delete(rawMemory);
		}
		
	//placement new/delete
	static void* operator new(std::size_t size,void* ptr) throw();
		{
			return ::operator new(size,ptr);	
		}
	static void operator delete(void *rawMemory,void* ptr) throw(); 
		{
		return ::operator delete(rawMemory,ptr);
		}
	
	//nothrow new/delete
	static void* operator new(std::size_t size,const std::nothrow_t& nt) throw();
		{
			return ::operator new(size,nt);	
		}
	static void operator delete(void *rawMemory,const std::nothrow_t& nt) throw(); 
		{
		return ::operator delete(rawMemory,nt);
		}
};

class Widget:public StandardNewDeleteForm{
	public:
		using StandardNewDeleteForm::operator new;
		using StandardNewDeleteForm::operator delete;
			
		static void* operator new(std::size_t size,std::ostream& logstream) throw(std::bad_alloc);// "placement new"
		static void operator delete(void *rawMemory, std::ostream&logStream) throw();// "placement delete"
			
		
	
};