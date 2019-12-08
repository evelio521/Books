/*在资源管理类中小心copying行为*/

void lock(Mutex* pm);//锁定pm指向的互斥器
void unlock(Mutex* pm);//互斥器解除锁定

class Lock{
	
	public:
		explicit Lock(Mutex*pm):mutexPtr(pm)
			{
				lock(mutexPtr);
			}
		~Lock()
		｛
				unlock(mutexPtr);
	    }
	private:
		Mutex *mutexPtr;
		
	
};

Mutex m;//定义互斥器
...
{
	
	Lock m1(&m);
	...
}

//如果发生下列行为 结果不可预知
Lock m1(&m);
Lock m2(m1);


//解决办法1
	class Lock:private Uncopyable{
	
	public:
		explicit Lock(Mutex*pm):mutexPtr(pm)
			{
				lock(mutexPtr);
			}
		~Lock()
		｛
				unlock(mutexPtr);
	    }
	private:
		Mutex *mutexPtr;
		
	
};

//解决方法2
class Lock{
	
	public:
		explicit Lock(Mutex*pm):mutexPtr(pm,unlock)
			{
				lock(mutexPtr.get());
			}
		
	private:
		std::tr1::shared_ptr<Mutex> mutexPtr;
		
	
};