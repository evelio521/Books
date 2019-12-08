 #include<iostream>
#include<list>
 class PhoneNumber{
 	
 	};
 class ABEntry{
 	
 	private:
		std::string theName;
		std::string theAddress;
		std::list<PhoneNumber> thePhones;
 		int numTimesconsulted;
 	public:
		ABEntry(const std::string& name,const  std::string &address,const  std::list<PhoneNumber>& phones);
 		
 		
};
//下面的只是赋值  不是初始化  尽量不要使用这种写法
/*
ABEntry::ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones)
	{
		
		theName=name;
		theAddress=address;
		thePhones=phones;
		numTimeconsulted=0;
	}
	*/
//初始化的写法  尽量采用这种写法

ABEntry::ABEntry(const  std::string& name,const  std::string &address,const  std::list<PhoneNumber>& phones):
	theName(name),theAddress(address),thePhones(phones),numTimesconsulted(0)
	{
		
	}
	
   class FileSystem{
   	
   	public:
   		size_t numDisks() const;
  };
   size_t FileSystem::numDisks()const
   {
	   return 5;//为了好些函数 这样写并不好 在实际中不应该这样写
   }
  FileSystem& tfs()
  {
  	static FileSystem fs;
  	return fs;
  }
  class Directory{
	public:
	  Directory();
   
  };//同上
  Directory::Directory()
  	{
  		size_t disks=tfs().numDisks();//这里tfs已经初始化
  	}
  Directory& tempDir()
  {
  	static Directory td;
  	return td;
  }
  
 int main()
 
 {
	std::list<PhoneNumber> ph;
 	ABEntry abe("nihao","jilin",ph);


 	return 0;
}