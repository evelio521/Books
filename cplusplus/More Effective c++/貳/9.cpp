	/*利用destructors避免资源泄漏*/
	
	//
	class ALA{
		public:
			virtual void processAdoption()=0;
					
	};
	class Puppy:public:ALA{
		
		public:
			virtual void processAdoption();
		
	};
	class Kitten:public:ALA{
		
		public:
			virtual void processAdoption();
		
	};
	ALA *read(istream& s);//从s 读取信息，返回一个指针，只想一个新分配的对象
	void processAdoptions(istream& dataSource)
	{
		
		while(dataSource){//如果还有数据
			ALA *pa=readALA(dataSource)//取出下一只动物
			pa->processAdoption();//处理收养事宜  此处很容易抛出异常
			delete pa;//删除readALA返回的对象
		}
	}
	
	//解决方法
	void processAdoptions(istream& dataSource)
	{
		
		while(dataSource){//如果还有数据
			auto_ptr<ALA> pa=readALA(dataSource)//取出下一只动物
			
			pa->processAdoption();//处理收养事宜  
			//auto_ptr会自动释放资源
		}
	}
	
