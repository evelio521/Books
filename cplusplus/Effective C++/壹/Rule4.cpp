/*确定对象使用前已经初始化*/


 class PhoneNumber{
 	
 	};
 class ABEntry{
 	
 	private:
 		string theName;
 		string theAddress;
 		list<PhoneNumber> thePhones;
 		int numTimesconsulted;
 	public:
 		ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones);
 		
 		
};
//下面的只是赋值  不是初始化  尽量不要使用这种写法

ABEntry::ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones)
	{
		
		theName=name;
		theAddress=address;
		thePhones=phones;
		numTimeconsulted=0;
	}
	
//初始化的写法  尽量采用这种写法

ABEntry::ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones):
	theName(name),theAddress(address),thePhones(phones),numTimeconsulted(0)
	{
		
	}
	
//不同编译单元内定义的non-local static对象的初始化顺序


  //在一个文件中
   class FileSystem{
   	
   	public:
   		size_t numDisks() const;
  };
  extern FileSystem tfs;
  //在另一个文件中
   class Directory{
   	public:
   		Directory(params);
  };
  Directory::Directory(params)
  	{
  		size_t disks=tfs.numDisks();//这里有可能tfs还没有初始化
  	}
  	
 //解决上述问题的解决办法
   class FileSystem{
   	
  };//同上
  FileSystem& tfs()
  {
  	static FileSystem fs;
  	return fs;
  }
  class Directory{
   
  };//同上
  Directory::Directory(params)
  	{
  		size_t disks=tfs().numDisks();//这里tfs已经初始化
  	}
  Directory& tempDir()
  {
  	static Dirrectory td;
  	return td;
  }
  
  
  
  