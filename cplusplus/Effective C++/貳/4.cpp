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
//�����ֻ�Ǹ�ֵ  ���ǳ�ʼ��  ������Ҫʹ������д��
/*
ABEntry::ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones)
	{
		
		theName=name;
		theAddress=address;
		thePhones=phones;
		numTimeconsulted=0;
	}
	*/
//��ʼ����д��  ������������д��

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
	   return 5;//Ϊ�˺�Щ���� ����д������ ��ʵ���в�Ӧ������д
   }
  FileSystem& tfs()
  {
  	static FileSystem fs;
  	return fs;
  }
  class Directory{
	public:
	  Directory();
   
  };//ͬ��
  Directory::Directory()
  	{
  		size_t disks=tfs().numDisks();//����tfs�Ѿ���ʼ��
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