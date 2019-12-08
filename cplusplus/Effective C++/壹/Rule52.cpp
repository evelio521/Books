����:52
/*д��placement new ҲҪдplacement delete*/
//operator new��operator deleteҪ�ɶ�ʹ�ã���������ָ��ʽ��Ҫ���䣬�ڱ�����new��deleteҲҪ�Ƕ�Ӧ��

// normal forms of new & delete
void* operator new(std::size_t) throw(std::bad_alloc);
void operator delete(void *rawMemory) throw();  // normal signature at global scope
void operator delete(void *rawMemory,std::size_t size) throw();  // normal signature at class scope

// placement version of new & delete
void* operator new(std::size_t, std::ostream& logStream) throw(std::bad_alloc);   // "placement new"
void operator delete(void *rawMemory, std::ostream&) throw();// "placement delete"

//�������һ�������������operator newû�д���������Ķ�Ӧ�汾��operator delete ��
//��ô�ڴ���䶯����Ҫȡ�����ָ��ɹ�ʱ��û���κ�operator delete�ᱻ����

class Widget{
	
	public:
		...
		static void* operator new(std::size_t size,std::ostream& logstream) throw(std::bad_alloc);// "placement new"
			static void operator delete(void *pMemory) throw();//normal signature 
			static void operator delete(void *rawMemory, std::ostream&logStream) throw();// "placement delete"
				...
					
};
Widget *pw=new(std::cerr) Widget;//�����ڴ�й©
delete pw;//����������operator delete


//��ֹ������������������
class StandardNewDeleteForm{
	//�ں����е�������ʽ��new��delete
	
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