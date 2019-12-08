//�������������ʱ��С����Դ��������copying��Ϊ

void lock(Mutex* pm);//����pmָ��Ļ�����
void unlock(Mutex* pm);//�������������

class Lock{
	
	public:
		explicit Lock(Mutex*pm):mutexPtr(pm)
			{
				lock(mutexPtr);
			}
		~Lock()
		��
				unlock(mutexPtr);
	    }
	private:
		Mutex *mutexPtr;
		
	
};

Mutex m;//���廥����
...
{
	
	Lock m1(&m);
	...
}

//�������������Ϊ �������Ԥ֪
Lock m1(&m);
Lock m2(m1);


//����취1
	class Lock:private Uncopyable{//��ֹ����
	
	public:
		explicit Lock(Mutex*pm):mutexPtr(pm)
			{
				lock(mutexPtr);
			}
		~Lock()
		��
				unlock(mutexPtr);
	    }
	private:
		Mutex *mutexPtr;
		
	
};

//�������2
class Lock{//ʹ��shared_ptr����
	
	public:
		explicit Lock(Mutex*pm):mutexPtr(pm,unlock)
			{
				lock(mutexPtr.get());
			}
		
	private:
		std::tr1::shared_ptr<Mutex> mutexPtr;
		
	
};