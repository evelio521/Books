/*ȷ������ʹ��ǰ�Ѿ���ʼ��*/


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
//�����ֻ�Ǹ�ֵ  ���ǳ�ʼ��  ������Ҫʹ������д��

ABEntry::ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones)
	{
		
		theName=name;
		theAddress=address;
		thePhones=phones;
		numTimeconsulted=0;
	}
	
//��ʼ����д��  ������������д��

ABEntry::ABEntry(const string& name,const string &address,const list<PhoneNumber>& phones):
	theName(name),theAddress(address),thePhones(phones),numTimeconsulted(0)
	{
		
	}
	
//��ͬ���뵥Ԫ�ڶ����non-local static����ĳ�ʼ��˳��


  //��һ���ļ���
   class FileSystem{
   	
   	public:
   		size_t numDisks() const;
  };
  extern FileSystem tfs;
  //����һ���ļ���
   class Directory{
   	public:
   		Directory(params);
  };
  Directory::Directory(params)
  	{
  		size_t disks=tfs.numDisks();//�����п���tfs��û�г�ʼ��
  	}
  	
 //�����������Ľ���취
   class FileSystem{
   	
  };//ͬ��
  FileSystem& tfs()
  {
  	static FileSystem fs;
  	return fs;
  }
  class Directory{
   
  };//ͬ��
  Directory::Directory(params)
  	{
  		size_t disks=tfs().numDisks();//����tfs�Ѿ���ʼ��
  	}
  Directory& tempDir()
  {
  	static Dirrectory td;
  	return td;
  }
  
  
  
  